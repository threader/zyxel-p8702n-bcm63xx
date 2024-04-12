BEGIN {
    FS = "="
    First = 1
    DisableCheckStatus = 1
    Debug = 0
}
function ClearAll() {
    DefaultVendor = ""
    DefaultProduct = ""
    TargetClass = ""
    MessageContent = ""
    NeedResponse = ""
    DetachStorageOnly = ""
    HuaweiMode = ""
    SierraMode = ""
    SonyMode = ""
    GCTMode = ""
    ResetUSB = ""
    Interface = ""
    Configuration = ""
    AltSetting = ""
    InquireDevice = ""
    CheckSuccess = ""
}

function PrintOutput() {
    Id = DefaultVendor ":" DefaultProduct ":"
    Param = ""
    if (ItemCount == 2) {
	if (Id2 == "")
	    Id2 = Id
	else
	    Id3 = Id
    } else {
	if (TargetClass != "")
	    Param = Param "-C " TargetClass " "
	if (NeedResponse != "")
	    Param = Param "-n "
	if (DetachStorageOnly != "")
	    Param = Param "-d "
	if (HuaweiMode != "")
	    Param = Param "-H "
	if (SierraMode != "")
	    Param = Param "-S "
	if (SonyMode != "")
	    Param = Param "-O "
	if (GCTMode != "")
	    Param = Param "-G "
	if (ResetUSB != "")
	    Param = Param "-R "
	if (Interface != "")
	    Param = Param "-i " Interface " "
	if (Configuration != "")
	    Param = Param "-u " Configuration " "
	if (AltSetting != "")
	    Param = Param "-a " AltSetting " "
	if (InquireDevice != "")
	    Param = Param "-I "
	if (CheckSuccess != "")
	    if (DisableCheckStatus != 1)
		Param = Param "-s " CheckSuccess " "
	if (MessageContent != "")
	    Param = Param "-M " MessageContent " "
    }

    if (Param != "") {
	if (Id2 != "") {
	    print Id2 Param
	    Id2 = ""
	}
	if (Id3 != "") {
	    print Id3 Param
	    Id3 = ""
	}
	print Id Param
    }
}

/;DefaultVendor/ {
    if (First == 0) {
	PrintOutput()
    } else {
	First = 0
    }

    ItemCount = 1
    ClearAll()

    DefaultVendor = $2
    gsub(" ", "", DefaultVendor)
    gsub(";", "", DefaultVendor)
    gsub("0x", "", DefaultVendor)
    if (Debug == 1)
	print ("DefaultVendor = " DefaultVendor)
}
/;DefaultProduct/ {
    ItemCount += 1    
    DefaultProduct = $2
    gsub(" ", "", DefaultProduct)
    gsub("0x", "", DefaultProduct)
    if (Debug == 1)
	print ("DefaultProduct = " DefaultProduct)
}
/;TargetClass/ {
    ItemCount += 1
    TargetClass = $2
    gsub(" ", "", TargetClass)
    gsub(";", "", TargetClass)
    gsub("0x", "", TargetClass)
    if (Debug == 1)
	print ("TargetClass = " TargetClass)
}
/;MessageContent/ {
    ItemCount += 1
    MessageContent = $2
    gsub(" ", "", MessageContent)
    gsub(";", "", MessageContent)
    gsub("\"", "", MessageContent)
    if (Debug == 1)
	print ("MessageContent = " MessageContent)
}
/;NeedResponse/ {
    ItemCount += 1
    NeedResponse = $2
    gsub(" ", "", NeedResponse)
    gsub(";", "", NeedResponse)
    if (Debug == 1)
	print ("NeedResponse = " NeedResponse)
}
/;DetachStorageOnly/ {
    ItemCount += 1
    DetachStorageOnly = $2
    gsub(" ", "", DetachStorageOnly)
    gsub(";", "", DetachStorageOnly)
    if (Debug == 1)
	print ("DetachStorageOnly = " DetachStorageOnly)
}
/;HuaweiMode/ {
    ItemCount += 1
    HuaweiMode = $2
    gsub(" ", "", HuaweiMode)
    gsub(";", "", HuaweiMode)
    if (Debug == 1)
	print ("HuaweiMode = " HuaweiMode)
}
/;SierraMode/ {
    ItemCount += 1
    SierraMode = $2
    gsub(" ", "", SierraMode)
    gsub(";", "", SierraMode)
    if (Debug == 1)
	print ("SierraMode = " SierraMode)
}
/;SonyMode/ {
    ItemCount += 1
    SonyMode = $2
    gsub(" ", "", SonyMode)
    gsub(";", "", SonyMode)
    if (Debug == 1)
	print ("SonyMode = " SonyMode)
}
/;GCTMode/ {
    ItemCount += 1
    GCTMode = $2
    gsub(" ", "", GCTMode)
    gsub(";", "", GCTMode)
    if (Debug == 1)
	print ("GCTMode = " GCTMode)
}
/;ResetUSB/ {
    ItemCount += 1
    ResetUSB = $2
    gsub(" ", "", ResetUSB)
    gsub(";", "", ResetUSB)
    if (Debug == 1)
	print ("ResetUSB = " ResetUSB)
}
/;Interface/ {
    ItemCount += 1
    Interface = $2
    gsub(" ", "", Interface)
    gsub(";", "", Interface)
    if (Debug == 1)
	print ("Interface = " Interface)
}
/;Configuration/ {
    ItemCount += 1
    Configuration = $2
    gsub(" ", "", Configuration)
    gsub(";", "", Configuration)
    if (Debug == 1)
	print ("Configuration = " Configuration)
}
/;AltSetting/ {
    ItemCount += 1
    AltSetting = $2
    gsub(" ", "", AltSetting)
    gsub(";", "", AltSetting)
    if (Debug == 1)
	print ("AltSetting = " AltSetting)
}
/;InquireDevice/ {
    ItemCount += 1
    InquireDevice = $2
    gsub(" ", "", InquireDevice)
    gsub(";", "", InquireDevice)
    if (Debug == 1)
	print ("InquireDevice = " InquireDevice)
}
/;CheckSuccess/ {
    ItemCount += 1
    CheckSuccess = $2
    gsub(" ", "", CheckSuccess)
    gsub(";", "", CheckSuccess)
    if (Debug == 1)
	print ("CheckSuccess = " CheckSuccess)
}

END {
    if (First == 0) {
	PrintOutput()
    }
}