
use strict;
use warnings;

require 5.10.0;

package BRCM::UnifdefPlus;
use Storable qw(dclone);

my $showErrs=1;

use Text::Balanced qw (
  extract_delimited
  extract_bracketed
  extract_quotelike
  extract_codeblock
  extract_variable
  extract_tagged
  extract_multiple
  gen_delimited_pat
  gen_extract_tagged
);

#use unique names (TRUE and FALSE can cause the code to change...)

my $TRUE  = "__unifdef_plus_TRUE__";
my $FALSE = "__unifdef_plus_FALSE__";
my $Y = qw(__unkconfig_y__);
my $M = qw(__unkconfig_m__);
my $N = qw(__unkconfig_n__);
my $COMMENT = qr/^\s*#.*$/;
my $BLANK_LINE = qr/^\s*$/;

my $BRACE_MATCH;
$BRACE_MATCH = qr/ (?: \w++ | \s++ | [\|&!=\"\\]++ | \((??{$BRACE_MATCH})\))* /x;


sub trim($) {
    my $string = shift;
    $string =~ s/^\s+//;
    $string =~ s/\s+$//;
    return $string;
}

sub trimWs($) {
    my $string = shift;
    #my ($ws1, $ws2);
    $string =~ /^(\s*)(\S*(?:\s+\S+)*)(\s*)$/;
    return ($1,$2,$3);
}


# Note: taken from
#http://en.cppreference.com/w/cpp/language/operator_precedence
my %opPrecidences = (
    '!'  => 3,
    '==' => 9,
    '!=' => 9,
    '&&' => 13,
    '||' => 14,

    # NOT YET SUPPORTED:

    # "<" => 8,
    # ">" => 8,
    # ">=" => 8,
    # "<=" => 8,
    # "+" => 6,
    # "-" => 6,
    # "*" => 5,
    # "/" => 5,
    # "%" => 5,
    # "&" => 10,
    # "^" => 11,
);

my $MAX_OP_PREC = 30;

my $EXPR = {
    'C' => {
        IF     => qr/^(\s*#if\s*)(.*?)\s*$/s,
        ELSEIF => qr/^(\s*#elif\s*)(.*?)\s*$/s,
        ELSE   => qr/^(\s*#else\s*)(.*?)\s*$/s,
        ENDIF  => qr/^(\s*#endif\s*)(.*?)\s*$/s,
        IFDEF  => qr/^(\s*#ifdef\s+)(.*?)\s*$/s,
        IFNDEF => qr/^(\s*#ifndef\s+)(.*?)\s*$/s,
        START_ML_COMMENT =>
qr{^((("(\\\\.|[^"\\\\])*")|(\\\'(\\\\.|[^\\\\])*\\\')|[^\'"/])*/\*)(\**[^\*/]|[^\*])*$}s,
        END_ML_COMMENT => qr{\*/(\**[^\*/]|[^\*])*$}s,
        LIT_IF     => "#if ",
        LIT_ELSE   => "#else ",
        LIT_ELSEIF => "#elif ",
        LIT_ENDIF  => "#endif ",
    },
    'Makefile' => {
        IF     => qr/^(\s*ifdef\s+BCM_KF\s*#\s*)(.*?)\s*$/s,
        ELSEIF => qr/^___NOT_MATCHABLE_STRING___$/s,
        ELSE   => qr/^(\s*else\s*#\s*BCM_KF\s*)(.*?)\s*$/s,
        ENDIF  => qr/^(\s*endif\s*#\s*BCM_KF\s*)(.*?)\s*$/s,
        IFDEF  => qr/^___NOT_MATCHABLE_STRING___$/s,
        IFNDEF => qr/^___NOT_MATCHABLE_STRING___$/s,
        START_ML_COMMENT => qr/^___NOT_MATCHABLE_STRING___$/s,
        END_ML_COMMENT => qr/^___NOT_MATCHABLE_STRING___$/s,
        LIT_IF     => "ifdef BCM_KF #  ",
        LIT_ELSE   => "else # BCM_KF ",
        LIT_ENDIF  => "endif # BCM_KF",
    },
    'Kconfig' => {
    }
};

#TBD: these do not account for two comments or C++ style comments on the same line...

my $OUTFILE;

# the following is a whitespace character sequence used to replace a \
# line ending.
my $LINE_CONT = "\r\r\n";

sub new {
    my $class = shift;
    my %opts  = @_;
    my $self  = {};
    $self->{lang} = $opts{lang} || 'C';

    $self->{dbg}       = $opts{dbg};
    $self->{defines}   = dclone( $opts{defines} );
    $self->{undefines} = dclone( $opts{undefines} );
    $self->{error}     = '';
    bless( $self, $class );
    return $self;
}

sub parse {
    my $self   = shift;
    my %opts   = @_;
    my $INFILE = $opts{INFILE};
    $OUTFILE = $opts{OUTFILE};
    my @inlines = <$INFILE>;
    $self->{inlines}     = \@inlines;
    $self->{wasModified} = 0;
    $self->{lang} = $opts{lang} || $self->{lang};
    $self->{infile} = $INFILE;
  
    if ($self->{lang} eq 'Kconfig') {
        my @undefineKeys = keys( %{ $self->{undefines} } );
        $self->{kDefines} = {};
        foreach (@undefineKeys) {
            my $modKey = $_;
            $modKey =~ s/^CONFIG_//;
            $self->{kDefines}{$modKey} = $N;
        }
        while(1) {
            my @outEntry = ();
            my $isVisible = $self->kconfigReadNextEntry(\@outEntry);
            if ($isVisible == 2) {            
                $self->{error} = "Unexpected end condition at line $..  Aborting";
                print STDERR "".$self->{error}."\n" if($showErrs);
                return;
            }
            if ($isVisible) {
                print $OUTFILE @outEntry;
            }
            else
            {
                $self->{wasModified} = 1;
            }
            last if (scalar(@{$self->{inlines}}) <= 0);
        }
    }
    else {
        if ( ! defined($EXPR->{$self->{lang}} )) {
            $self->{error} = "Error: unknown language $self->{lang}.\n";
            print STDERR "".$self->{error}."\n" if($showErrs);
            return;
        }
    
        my ( $closingExpr, $closingArg ) = $self->parseLines(1);

        if ($closingExpr) {
            $self->{error} =
              "Error: unexpected $closingExpr $closingArg at line $..  Aborting";
            print STDERR "".$self->{error}."\n" if($showErrs);
            return;
        }
    }
    return ( $self->{wasModified} );
}

sub parseCondition {
    my $self = shift;
    my $lang = $self->{lang};

    # replace known variables
    my $condition     = shift;
    my $origCondition = $condition;

    my @defineKeys = keys( %{ $self->{defines} } );
    foreach (@defineKeys) {
        my $defineKey = $_;
        $condition =~ s/defined\s*\(\s*$defineKey\s*\)/$TRUE/g;

       #This part doesn't work if $defineKey is a substring of larger macro name
       #$condition =~ s/$defineKey/$defines{$defineKey}/g;
    }

    my @undefineKeys = keys( %{ $self->{undefines} } );
    foreach (@undefineKeys) {
        my $undefineKey = $_;
        $condition =~ s/defined\s*\(\s*$undefineKey\s*\)/$FALSE/g;
    }

    if ( $condition eq $origCondition ) {
        print $OUTFILE "Not simplifying $condition\n" if $self->{dbg};
        return $condition;
    }

    print $OUTFILE "simplifying $condition\n" if $self->{dbg};

    my $remainder;
    ( $condition, $remainder ) =
      $self->simplifyExpr( $condition, $MAX_OP_PREC, 0 );

    if ( !$condition ) {
        print STDERR "Could not parse condition: $condition\n";
        return $origCondition;
    }
    else {
        $self->{wasModified} = 1;
        return $condition;
    }
}

sub simplifyExpr {
    my $self          = shift;
    my $string1       = shift;
    my $currentOpPrec = shift;
    my $level         = shift;
    my $lang          = $self->{lang};

    $level += 1;

    my $i      = 0;
    my $dbgStr = "(line=$., $currentOpPrec)>";
    while ( $i < $level ) {
        $dbgStr .= "  ";
        $i++;
    }

    my $remainder;

    my $operand1;
    my $operator;
    my $operand2;

    my $ws_bo1;    #whitespace before operand1
    my $ws_ao1;    #whitespace after operand1
    my $ws_bo2;    #whitespace before operand2
    my $ws_ao2;    #whitespace after operand2

    print $OUTFILE
"\n$dbgStr Simplifying expression $string1 ($currentOpPrec) (level=$level)\n"
      if $self->{dbg};

    #read first operand:

    #test for brackets:
    if ( $string1 =~ /^(\s*)\((.*)$/s ) {
        my $braceExpr;
        my $remainder2;
        my $ws_bb = $1;    #whitespace before brackets
                           #get expression within braces
        ( $braceExpr, $remainder ) = extract_bracketed( $string1, "()" );
        $braceExpr =~ s/^\((.*)\)$/$1/s;

        #simplify expression within braces
        ( $braceExpr, $remainder2 ) =
          $self->simplifyExpr( $braceExpr, $MAX_OP_PREC, $level );
        print $OUTFILE "$dbgStr WARNING: raminder2 not null ($remainder2)\n" if ($remainder2!~/^\s*$/);

        #if brace expression has exactly one term, remove braces:

        if ( $braceExpr =~ /^\s*\w+\s*$/s ) {
            $operand1 = $braceExpr;
            $ws_bo1   = $ws_bb;
        }
        elsif ( $braceExpr =~ /^\s*\w+\s*\([^()]*\)\s*$/s ) {

            #known shortcoming:  this will not simplify (XXX(2*(YYY+ZZZ))
            $operand1 = $braceExpr;
            $ws_bo1   = $ws_bb;
        }
        else {
            print $OUTFILE "$dbgStr Not removing braces\n" if $self->{dbg};
            $operand1 = "($braceExpr)";
            $ws_bo1   = $ws_bb;
        }
    }

    #test for not operator:
    elsif ( $string1 =~ /^(\s*)!(\s*)(.*)$/s ) {
        $ws_bo1 = $1;
        my $ws_an = $2;    #whitespace after not
        my $notOperand;

#known shortcoming: the following does not test for hex numbers, or longs (0x0l, etc).
        ( $notOperand, $remainder ) =
          $self->simplifyExpr( $3, $opPrecidences{"!"} + 1, $level )
          or return;
        if ( $notOperand =~ /^(\s*)(($FALSE)|(0))(\s*)$/s ) {
            $operand1 = "$TRUE$5";
        }
        elsif ( $notOperand =~ /^(\s*)(($TRUE)|(d+))(\s*)$/s ) {
            $operand1 = "$FALSE$5";
        }
        else {
            $operand1 = "!" . $ws_an . $notOperand;
        }
    }
    else {
        print $OUTFILE "$dbgStr $string1\n" if $self->{dbg};
        if ( $string1 =~ /^(\s*)(\w+)(\s*)(.*)/s ) {
            $ws_bo1    = $1;
            $operand1  = $2;
            $ws_ao1    = $3;
            $remainder = $4;

            my $braceExpr = "";

#handle function -- if we have a word, followed by brackets, it's some sort of function/macro.
#add the bracketed expression to operand1
            ( $braceExpr, $remainder ) = extract_bracketed( $remainder, "()" );
            if ($braceExpr) {
                $operand1 .= $ws_ao1 . $braceExpr;
                $ws_ao1 = "";
            }
        }
    }

    print $OUTFILE
      "$dbgStr operand1 is   .$operand1.  remainder is  .$remainder.\n"
      if $self->{dbg};

#switch this to a while loop...  note this calls recusrively into itself for op2 each time...

    #if ( $remainder =~ /^\s*$/ ) {
    #    return ( $operand1, $remainder );
    #}

    while ( !( $remainder =~ /^\s*$/s ) ) {
        if ( $remainder =~ /(\s*)([\!\|\>\<\=&\/\*]+)(\s*)(.*)$/s ) {
            $ws_ao1 .= $1;
            $operator = $2;
            $ws_bo2 .= $3;
            my $remainder2 = $4;
            print $OUTFILE
              "$dbgStr operator is .$operator.  remainder2 is $remainder2\n"
              if $self->{dbg};
            if ( $operator eq "//" || $operator eq "/*" ) {
                $self->{error} =
"do not support comments within expressions ($operator$remainder2)";
                print STDERR "".$self->{error}."\n" if($showErrs);

                return;
            }
            if ( exists $opPrecidences{$operator} ) {
                if ( $opPrecidences{$operator} < $currentOpPrec ) {
                    print $OUTFILE
"$dbgStr operand1 is $operand1, operator is $operator, simplifying .$remainder2. ($opPrecidences{$operator})\n"
                      if $self->{dbg};

                    ( $operand2, $remainder ) =
                      $self->simplifyExpr( $remainder2,
                        $opPrecidences{$operator}, $level )
                      or return;
                    print $OUTFILE
                      "$dbgStr operand2 is .$operand2.  operator is $operator\n"
                      if $self->{dbg};

# Note, the followin scheme can probably be written a bit neater, but I'm new to perl...

# simplifying:
# TBD: create isTrue and isFalse functions here, such that isTrue(1) = isTrue(" $TRUE") = isTrue("TRUE")
                    if (
                        ( $operator eq "&&" )
                        && (   ( $operand1 =~ /^\s*$FALSE\s*$/s )
                            || ( $operand2 =~ /^\s*$FALSE(\s*)$/s ) )
                      )
                    {
                        print $OUTFILE
"$dbgStr $operand1 && $operand2 = $FALSE, r:$remainder\n"
                          if $self->{dbg};
                        $operand1 = "$FALSE";
                        $ws_bo1   = $ws_bo1;
                        $ws_ao1   = $ws_ao2;
                    }
                    elsif (( $operator eq "&&" )
                        && ( $operand1 =~ /^\s*$TRUE\s*$/s ) )
                    {
                        print $OUTFILE
"$dbgStr $operand1 && $operand2 = $operand2, r:$remainder\n"
                          if $self->{dbg};
                        $operand1 = $operand2;
                        $ws_bo1   = $ws_bo2;
                        $ws_ao1   = $ws_ao2;
                    }
                    elsif (( $operator eq "&&" )
                        && ( $operand2 =~ /^\s*$TRUE\s*$/s ) )
                    {
                        print $OUTFILE
"$dbgStr $operand1 && $operand2 = $operand1, r:$remainder\n"
                          if $self->{dbg};
                        $operand1 = $operand1;
                        $ws_bo1   = $ws_bo1;
                        $ws_ao1   = $ws_ao1;
                    }
                    elsif (
                        ( $operator eq "||" )
                        && (   ( $operand1 =~ /^\s*$TRUE\s*$/s )
                            || ( $operand2 =~ /^\s*$TRUE(\s*)$/s ) )
                      )
                    {
                        print $OUTFILE
"$dbgStr $operand1 || $operand2 = $TRUE, r:$remainder\n"
                          if $self->{dbg};
                        $operand1 = "$TRUE";
                        $ws_bo1   = $ws_bo1;
                        $ws_ao1   = $ws_ao2;
                    }
                    elsif (( $operator eq "||" )
                        && ( $operand1 =~ /^\s*$FALSE\s*$/s ) )
                    {
                        print $OUTFILE
"$dbgStr $operand1 || $operand2 = $operand2, r:$remainder\n"
                          if $self->{dbg};
                        $operand1 = $operand2;
                        $ws_bo1   = $ws_bo2;
                        $ws_ao1   = $ws_ao2;
                    }
                    elsif (( $operator eq "||" )
                        && ( $operand2 =~ /^\s*$FALSE\s*$/s ) )
                    {
                        print $OUTFILE
"$dbgStr $operand1 || $operand2 = $operand1, r:$remainder\n"
                          if $self->{dbg};
                        $operand1 = $operand1;
                        $ws_bo1   = $ws_bo1;
                        $ws_ao1   = $ws_ao1;
                    }

              #elsif ( ($operator eq "==") && ($operand1 eq $operand2) ) {
              #    #known shortcoming -- this does not account for whitespace...
              #    return ("$TRUE", $remainder);
              #}
              #elsif ( ($operator eq "!=") && ($operand1 eq $operand2) ) {
              #    #known shortcoming -- this does not account for whitespace...
              #    return ("$FALSE, $remainder);
              #}
                    else {
                        print $OUTFILE
"$dbgStr Could not simplify: $operand1$operator$operand2, r:$remainder\n"
                          if $self->{dbg};
                        $operand1 =
                          $operand1 . $ws_ao1 . $operator . $ws_bo2 . $operand2;
                        $ws_bo1 = $ws_bo1;
                        $ws_ao1 = $ws_ao2;
                    }
                }
                else {
                    print $OUTFILE
"$dbgStr op precidence exceeded: returning $operand1, r:$remainder\n\n"
                      if $self->{dbg};
                    return ( $operand1, $remainder );
                }
                print $OUTFILE "$dbgStr operand1 = $operand1, r:$remainder\n"
                  if $self->{dbg};
            }
            else {
                $self->{error} = "operator not found .$operator.";
                print STDERR "".$self->{error}."\n" if($showErrs);
                return;    # error
            }
        }

        print $OUTFILE
"$dbgStr end while loop operand1=.$operand1., remainder=.$remainder.\n\n"
          if $self->{dbg};

        $operator = "";
        $operand2 = "";
    }

    return ( $operand1, $remainder );
}

sub parseIf {
    my $self       = shift;
    my $litExpr    = shift;
    my $expression = shift;
    my $keep       = shift;
    my $lang       = $self->{lang};
    my $nextExprType;
    my $nextConditional;
    my $line;

    if ( $expression =~ /\s*$TRUE\s*/ ) {

        # do not echo #if.
        # find next #else / #elseif, and wipe till #endif:

        ( $nextExprType, $nextConditional ) = $self->parseLines($keep);
        while ( $nextExprType =~ /$EXPR->{$lang}->{ELSEIF}/s ) {
            ( $nextExprType, $nextConditional ) = $self->parseLines(0);
            if (!$nextExprType) {
                $self->{error} = "unexpected end of file";
                print STDERR "".$self->{error}."\n" if($showErrs);
                return;
              }
        }
        if ( $nextExprType =~ /$EXPR->{$lang}->{ELSE}/s ) {
            ( $nextExprType, $nextConditional ) = $self->parseLines(0);
            if (!$nextExprType) {
                $self->{error} = "unexpected end of file";
                print STDERR "".$self->{error}."\n" if($showErrs);
                return;
              }
            if ( $nextExprType !~ /$EXPR->{$lang}->{ENDIF}/s ) {
                $self->{error} = "unterminated if/else ($expression)";
                print STDERR "".$self->{error}."\n" if($showErrs);
                return;
              }

            # do not echo endif.
        }
        else {
            if (!$nextExprType) {
                $self->{error} = "unexpected end of file";
                print STDERR "".$self->{error}."\n" if($showErrs);
                return;
              }
            if ( $nextExprType !~ /$EXPR->{$lang}->{ENDIF}/s ) {
                $self->{error} = "unterminated if ($expression)";
                print STDERR "".$self->{error}."\n" if($showErrs);
                return;
              }

            # do not echo endif.
        }
        return (1);
    }
    elsif ( $expression =~ /\s*$FALSE\s*/ ) {

        #do not echo #if.
        #if we find an elseif, treat it as an if
        #if we find an else, echo till endif (do not echo endif)

        my ( $nextExprType, $nextConditional ) = $self->parseLines(0);
        if (!$nextExprType) {
             $self->{error} = "unterminated if ($expression)";
             print STDERR "".$self->{error}."\n" if($showErrs);
             return;
        }
        
        if ( $nextExprType =~ /$EXPR->{$lang}->{ELSEIF}/s ) {
            $self->parseIf( $EXPR->{$lang}->{LIT_IF}, $nextConditional, $keep );
        }
        elsif ( $nextExprType =~ /$EXPR->{$lang}->{ELSE}/s ) {
            ( $nextExprType, $nextConditional ) = $self->parseLines($keep);
            if ( $nextExprType !~ /$EXPR->{$lang}->{ENDIF}/s ) {
                $self->{error} = "unterminated if ($expression)";
                print STDERR "".$self->{error}."\n" if($showErrs);
                return;
            }
        }
    }
    else {

        #undefined.  echo if.
        #if we find an elsif, treat it as an if
        #if we find an else, echo that.
        #echo endif

        $line = $litExpr . $expression;
        $line =~ s/$LINE_CONT/\\\n/g;

        print $OUTFILE $line . "\n" if $keep;
        ( $nextExprType, $nextConditional ) = $self->parseLines($keep);
        if (!$nextExprType) {
            $self->{error} = "Unmatched  $litExpr  $expression at eof";
            print STDERR "".$self->{error}."\n" if($showErrs);
            return;
          }

        while ( $nextExprType =~ /$EXPR->{$lang}->{ELSEIF}/s ) {
            my $nextExpression = $self->parseCondition($nextConditional);

            #my $nextExpression = $nextConditional;
            if ( $nextExpression =~ m/\s*$TRUE\s*/ ) {
                print $OUTFILE $EXPR->{$lang}->{LIT_ELSE} . "\n" if $keep;
                ( $nextExprType, $nextConditional ) = $self->parseLines($keep);
                while ( !( $nextExprType =~ /$EXPR->{$lang}->{ENDIF}/s ) ) {
                    if (!$nextExprType) {
                        $self->{error} = "unexpected end of file";
                        print STDERR "".$self->{error}."\n" if($showErrs);
                        return;
                      }
                    ( $nextExprType, $nextConditional ) = $self->parseLines(0);
                }
            }
            elsif ( $nextExpression =~ /\s*$FALSE\s*/ ) {
                ( $nextExprType, $nextConditional ) = $self->parseLines(0);
            }
            else {
                $line = $EXPR->{$lang}->{LIT_ELSEIF} . $nextExpression;
                $line =~ s/$LINE_CONT/\\\n/g;
                print $OUTFILE $line . "\n" if $keep;
                ( $nextExprType, $nextConditional ) = $self->parseLines($keep);
            }
        }
        if ( $nextExprType =~ /$EXPR->{$lang}->{ELSE}/s ) {
            $line = $nextExprType . $nextConditional;
            $line =~ s/$LINE_CONT/\\\n/g;
            print $OUTFILE $line . "\n" if $keep;
            ( $nextExprType, $nextConditional ) = $self->parseLines($keep);
        }
        if ( $nextExprType !~ /$EXPR->{$lang}->{ENDIF}/s ) {
            $self->{error} = "expected endif ($nextExprType)";
            print STDERR "".$self->{error}."\n" if($showErrs);
            return;
}
        $line = $nextExprType . $nextConditional;
        $line =~ s/$LINE_CONT/\\\n/g;
        print $OUTFILE $line . "\n" if $keep;
    }

    return (1);
}

sub parseLines {
    my $self = shift;
    my $keep = shift;
    my $lang = $self->{lang};
    my $line;
    my $newLine;
    my $litExpr;
    my $expression;

    while (1) {

        $line = ( shift @{ $self->{inlines} } );
        return unless defined($line);
        chomp $line;

        while ( $line =~ /^(.*)\\$/s ) {

#switch \ at end of line to whitespace -- but we will need to recover it later on,
#
            $line = $1 . $LINE_CONT . ( shift @{ $self->{inlines} } );
            chomp $line;
        }
        if ( $line =~ m#$EXPR->{$lang}->{START_ML_COMMENT}#s ) {

            # Treat multi-line comments as a single line
            while ( $newLine = ( shift @{ $self->{inlines} } ) ) {
                chomp $newLine;
                $line = $line . "\n" . $newLine;
                last if $newLine =~ m#$EXPR->{$lang}->{END_ML_COMMENT}#s;
            }
        }
        if ( $line =~ /$EXPR->{$lang}->{IFDEF}/s ) {
            $litExpr    = $1;
            $expression = $2;

            #$macro = $expression =~ /^\s+(\S+)/s ;
            my $macro = trim($expression);
            print $OUTFILE
              "litExpr=$litExpr, expression=$expression, macro=.$macro.\n"
              if $self->{dbg};
            if ( $self->{defines}->{$macro} ) {
                print $OUTFILE "calling parseIf TRUE\n" if $self->{dbg};
                $self->parseIf( $EXPR->{$lang}->{LIT_IF}, "$TRUE", $keep );
            }
            elsif ( $self->{undefines}->{$macro} ) {
                print $OUTFILE "calling parseIf FALSE\n" if $self->{dbg};
                $self->parseIf( $EXPR->{$lang}->{LIT_IF}, "$FALSE", $keep );
            }
            else {
                print $OUTFILE "calling parseIf $expression\n" if $self->{dbg};
                $self->parseIf( $litExpr, $expression, $keep );
            }
        }
        elsif ( $line =~ /$EXPR->{$lang}->{IFNDEF}/s ) {
            $litExpr    = $1;
            $expression = $2;

            #$macro = $expression =~ /^\s+(\S+)/s ;
            my $macro = trim($expression);
            if ( $self->{defines}->{$macro} ) {
                $self->parseIf( $EXPR->{$lang}->{LIT_IF}, "$FALSE", $keep );
            }
            elsif ( $self->{undefines}->{$macro} ) {
                $self->parseIf( $EXPR->{$lang}->{LIT_IF}, "$TRUE", $keep );
            }
            else {
                $self->parseIf( $litExpr, $expression, $keep );
            }
        }
        elsif ( $line =~ /$EXPR->{$lang}->{IF}/s ) {
            $litExpr    = $1;
            $expression = $self->parseCondition($2);
            $self->parseIf( $litExpr, $expression, $keep );
        }
        elsif ( $line =~ /$EXPR->{$lang}->{ELSEIF}/s ) {
            $litExpr    = $1;
            $expression = $2;
            return ( $litExpr, $expression );
        }
        elsif ( $line =~ /$EXPR->{$lang}->{ELSE}/s ) {
            $litExpr    = $1;
            $expression = $2;
            return ( $litExpr, $expression );
        }
        elsif ( $line =~ /$EXPR->{$lang}->{ENDIF}/s ) {
            $litExpr    = $1;
            $expression = $2;
            return ( $litExpr, $expression );
        }
        else {

            # replace stripped out line cont's:
            $line =~ s/$LINE_CONT/\\\n/g;
            print $OUTFILE $line . "\n" if $keep;
        }
    }
}

## --------------------------------------------------------------------------
## KCONFIG support:
##
## Note: Kconfig work very differently than Makefiles or C files
##
##
# NOTE: if behaviour is actually simplified a bit:  in reality, if appends a 
# condition to all items inside of it.  (So, if you had an 
#
#    if x
#    source Kconfig.foo
#    endif
#
# then source Kconfig.foo would still get expanded, only every entry in
# Kconfig.foo would have if x appended to its end. This code treats if
# differently, in that it would remove source Kconfig.foo entirely if x 
# was false...

sub kconfigSimplifyExprTop {
    my $self = shift;
    my $expr = shift;

    my %realval = ( $Y => "y", $M => "m", $N => "n");
    my $check = join '|', keys %realval;
    
    $expr = $self->kconfigSimplifyExpr($expr);
    $expr =~ s/($check)/$realval{$1}/g;
    return $expr;
}

sub kconfigSimplifyExpr {
    my $self = shift;
    my $expr = shift;

    my ($ws1, $ws2, $ws3, $ws4, $ws5, $ws6, $op1, $op2);
    my ($ws11, $ws12, $ws21, $ws22);
    my ($e1, $e2);

    if ($expr =~ /^($BRACE_MATCH)\|\|($BRACE_MATCH)$/ ) {
        ($e1, $e2) = ($1,$2);
        ($ws11,$op1,$ws12) = trimWs($self->kconfigSimplifyExpr($e1)); 
        ($ws21,$op2,$ws22) = trimWs($self->kconfigSimplifyExpr($e2));
        return $Y if ($op1 eq $Y || $op2 eq $Y);
        return $M if (($op1 eq $M || $op1 eq $Y) && ($op2 eq $M || $op2 eq $Y));
        return $N if ($op1 eq $N && $op2 eq $N);
		return "".$ws11.$op1 if ($op2 eq $N);
		return "".$ws11.$op2 if ($op1 eq $N);
		return "".$ws11.$op1.$ws12."||".$ws21."m" if ($op2 eq $M);
		return "".$ws11."m".$ws12."||".$ws21.$op2 if ($op1 eq $M);   
		return "".$ws11.$op1.$ws12."||".$ws21.$op2.$ws22;
    }
	elsif ($expr =~ /^($BRACE_MATCH)&&($BRACE_MATCH)$/ ) {
		($e1, $e2) = ($1,$2);
		($ws11,$op1,$ws12) = trimWs($self->kconfigSimplifyExpr($e1)); 
		($ws21,$op2,$ws22) = trimWs($self->kconfigSimplifyExpr($e2));
        return $Y if ($op1 eq $Y && $op2 eq $Y);
        return $M if (($op1 eq $M || $op1 eq $Y) && ($op2 eq $M || $op2 eq $Y));
        return $N if ($op1 eq $N || $op2 eq $N);
		return "".$ws11.$op1 if ($op2 eq $Y);
		return "".$ws11.$op2 if ($op1 eq $Y);
		return "".$ws11.$op1.$ws12."&&".$ws21."m".$ws22 if ($op2 eq $M);
		return "".$ws11."m".$ws12."&&".$ws21.$op2 if ($op1 eq $M);
		return "".$ws11.$op1.$ws12."&&".$ws21.$op2.$ws22;
    }
	elsif ($expr =~ /^(\s*)!($BRACE_MATCH)$/ ) {
		($ws1, $e1) =($1,$2);
		($ws11,$op1,$ws12) = trimWs($self->kconfigSimplifyExpr($e1));
        return $N if ($op1 eq $Y);
        return $M if ($op1 eq $M);
        return $Y if ($op1 eq $N);
		return "".$ws1."!".$ws11.$op1.$ws12;
    }
	elsif ($expr =~ /^(\s*)\(($BRACE_MATCH)\)(\s*)$/ ) {
		($ws1, $e1, $ws2) =($1,$2,$3);
		($ws11,$op1,$ws12) = trimWs($self->kconfigSimplifyExpr($e1));
		return $ws1.$op1.$ws2 if ($op1 =~ /^\s*\w+\s*$/);
		return "".$ws1."(".$ws11.$op1.$ws12.")".$ws2;
    }
	elsif ($expr =~ /^($BRACE_MATCH)!=($BRACE_MATCH)$/ ) {
		($e1, $e2) = ($1,$2);
		($ws11,$op1,$ws12) = trimWs($self->kconfigSimplifyExpr($e1)); 
		($ws21,$op2,$ws22) = trimWs($self->kconfigSimplifyExpr($e2));
        if (    ($op1 eq $Y || $op1 eq $M || $op1 eq $N)
             && ($op2 eq $Y || $op2 eq $M || $op2 eq $N)) {
            return $N if ($op1 eq $op2);
            return $Y;
        }
        else {
			return "".$ws11.$op1.$ws12."!=".$ws21.$op2.$ws22;
        }              
    }
	elsif ($expr =~ /^($BRACE_MATCH)=($BRACE_MATCH)$/) {
		($e1, $e2) = ($1,$2);
		($ws11,$op1,$ws12) = trimWs($self->kconfigSimplifyExpr($e1)); 
		($ws21,$op2,$ws22) = trimWs($self->kconfigSimplifyExpr($e2));
        if (    ($op1 eq $Y || $op1 eq $M || $op1 eq $N)
             && ($op2 eq $Y || $op2 eq $M || $op2 eq $N)) {
            return $Y if ($op1 eq $op2);
            return $N;
        }
        else {
			return "".$ws11.$op1.$ws12."=".$ws21.$op2.$ws22;
        }              
    }
	elsif ($expr =~ /^(\s*)((?:\w|\")+)(\s*)$/) {
		return $1.$self->{kDefines}{$2}.$3 if defined $self->{kDefines}{trim($expr)};
        return $expr;
    }

    die "WARNING -- could not resolve <$expr>!!\n";
    return $expr;

}

my $LINE_SEP = "\r\r\r";

# returns the visible lenght of whitespace (assuming tabs are 8 characters wide)
sub wslength {
	my $str = shift;
	# code taken from eugene y on 
	# http://stackoverflow.com/questions/5997404/perl-program-to-replace-tabs-with-spaces
	while($str =~ s/\t/" " x (8 - $-[0]%8)/e) {}	
	return length($str);
}

# attempts to read attributes
# stops when a line without indents is found
sub kconfigReadAttributes {
    my $self = shift;
    my $outLinesRef = shift;

    # clear output:
    @$outLinesRef = ();
    my $isVisible = 1;

    while (1) { 
        my $line = shift(@{$self->{inlines}});
        last unless defined($line);
        chomp($line);
        # hack: combine two lines into one.
        while (substr($line, -1) eq "\\") {
            $line = substr($line,0,-1) . $LINE_SEP . shift(@{$self->{inlines}});
            chomp($line);
        }


        if ($line =~ $COMMENT) {
            #comment: just push it
            $line =~ s/$LINE_SEP/\\\n/g;
            push(@$outLinesRef, $line."\n");
        }
        elsif ($line =~ /^(\s*)$/) {
            #blank line -- end of attributes.  Include blank line as 
            #part of item.
            $line =~ s/$LINE_SEP/\\\n/g;
            push(@$outLinesRef, $line."\n");
            last;
        }
        elsif ($line =~ /^[^\s]/) {
            #line with no preceding whitespace.  Though the spec says there should
            #be whitespace between entries, this rule isn't followed everywhere.
            #unshif the line, and finish:
            unshift(@{$self->{inlines}}, $line);
            last;
        }
        elsif ($line =~ /^(\s+)-*help-*.*$/)
        {
            #special handling for help: it is multiline and ends when the
            #first line of text has less indentation than the first.
            push(@$outLinesRef, $line."\n");
            my $line = shift(@{$self->{inlines}});
            chomp($line);

            if ($line =~ /^(\s+)/) {
                # Grumble grumble: in at least one kernel Kconfig 
                # file, they mixed up tabs and spaces...:
                my $blankLines = 0;
                my $helpIndentLen = wslength($1);
                $line =~ s/$LINE_SEP/\\\n/g;
                push(@$outLinesRef, $line."\n");
                while (1) {
                    my $line = shift(@{$self->{inlines}});
                    last unless defined($line);
                    chomp($line);
                    if (length($line) == 0) {
                       $blankLines++; 
                    }
                    else {
                        $line =~ /^(\s*)/;
                        if (wslength($1) < $helpIndentLen) {
                            unshift(@{$self->{inlines}}, $line);
                            while($blankLines--) {unshift(@{$self->{inlines}}, "");};
                            last;
                        } else {
                            while($blankLines > 0) {$blankLines--; push(@$outLinesRef, "\n");};
                            $line =~ s/$LINE_SEP/\\\n/g;
                            push(@$outLinesRef, $line."\n");
                        }
                    } 
                }
            }
            else {
                # Grumble grumle -- apperently it's possible to have empty
                # help fields...
                unshift(@{$self->{inlines}}, $line);
            }
        }
        elsif ($line =~ /^(\s+depends on\s*)(.*)$/) {
            my $tmp = $1;
            my $condition = $self->kconfigSimplifyExprTop($2);
            if ($condition eq 'n') {
                $isVisible = 0;
            }
            else {
                $condition =~ s/$LINE_SEP/\\\n/g;
                push(@$outLinesRef, $tmp . $condition . "\n");  
            }
        }
        elsif ($line =~ /^(\s+\w+\s*.*)(\s+if\s+)(.*)$/) {
            my $term = $1;
            my $ifTerm = $2;
            my $condition = trim($self->kconfigSimplifyExprTop($3));
            if ($condition eq "n") {
            }
            elsif ($condition eq "y") {
                $term =~ s/$LINE_SEP/\\\n/g;
                push(@$outLinesRef, $term."\n");
            
            }
            else {          
                $term =~ s/$LINE_SEP/\\\n/g;
                $ifTerm =~ s/$LINE_SEP/\\\n/g;
                push(@$outLinesRef, $term . $ifTerm . $condition . "\n");
            }
        }
        elsif ($line =~ /^\s+(\w+)\s*(.*)$/) {
            $line =~ s/$LINE_SEP/\\\n/g;
            push(@$outLinesRef, $line."\n");
        }
        else  {
            $self->{error} = "Error parsing Kconfig file ($line)";          
            print STDERR "".$self->{error}."\n" if($showErrs);
            last;
        }
    }
    return $isVisible;
}

# return 0 invisible entry, return 1 visible entry, 2 for end-block command
sub kconfigReadNextEntry {
    my $self = shift;
    my $outLinesRef = shift;
    
    my $isVisible = 1;
    my $line;
    
    do {
        $line = ( shift @{ $self->{inlines} } );
        return 1 unless defined($line);
        chomp($line);
        # in the case of an unexpected endblock, push the endblock back onto the array,
        # and return 2.  The caller will process the endblock line.
        if ($line =~ "\^endchoice" || $line =~ "\^endmenu" || $line =~ "\^endif") {
            unshift(@{ $self->{inlines} }, $line."\n");
            return 2;
        }
        push(@$outLinesRef, $line."\n");        
    } while(    $line =~ /$COMMENT/ || $line =~ /$BLANK_LINE/ 
             || $line =~ /^source\s/ || $line =~ /^mainmenu\s/ );

    
    if (    $line =~ /^config\s/ || $line =~ /^menuconfig\s/ || $line =~ /^comment/ 
         || $line =~ /^choice/ || $line =~ /^menu/) {
        #read attributes
        my @entryAttributes = ();
        
        $isVisible = $self->kconfigReadAttributes(\@entryAttributes);
        $self->{wasModified} = 1 unless ($isVisible);
        push(@$outLinesRef, @entryAttributes) if ($isVisible);
    }

    my $endStr = "";
    $endStr = qw/endchoice/     if ($line =~ /^choice\s*$/);
    $endStr = qw/endmenu/       if ($line =~ /^menu\s/);
    $endStr = qw/endif/         if ($line =~ /^if\s/);

    if ($endStr)
    {
        #special handling for if: we need to modify the condition
        if ($line =~ /^if\s+(.*)$/) {
            my $condition = $self->kconfigSimplifyExprTop($1);
            if ($condition eq "n") {
                $isVisible = 0 ;
                $self->{wasModified} = 1
            } 
            elsif ($condition ne $1) {      
                # expression changed -- rewriting
                pop(@$outLinesRef);
                push(@$outLinesRef, "if ".$condition."\n")
            }
        }
        
        #read block:                
        while(1) {                  
            my @subEntryOutLines = ();
            last if (scalar(@{$self->{inlines}}) <= 0);
            my $isNextEntryVisible = $self->kconfigReadNextEntry(\@subEntryOutLines);
            if ($isNextEntryVisible != 0) {
                push(@$outLinesRef, @subEntryOutLines) if ($isVisible);
            }
            else {
                $self->{wasModified} = 1;
            }
            
            if ($isNextEntryVisible == 2) {
                #Get next entry found endmenu, endchoice or endif.
                #Last line was unshifted back into inLines
                my $closingLine = ( shift @{ $self->{inlines} } );

                if (!defined($closingLine)) {
                    $self->{error} = "Internal Error (1)\n";
                    print STDERR "".$self->{error}."\n" if($showErrs);
                    return;
                }
                if ( $closingLine =~ /^$endStr/) {
                    push(@$outLinesRef, $closingLine) if ($isVisible);
                    last;
                }
                else {
                    $self->{error} =  "Unexpected closing line: <".trim($closingLine)."> (expecting <$endStr>)\n";
                    print STDERR "".$self->{error}."\n" if($showErrs);
                    return;
                }   
            }
        }
    }
    return $isVisible;
}

1;

