
all: lib install


lib:
	cp ./examples/linux-user/bmd/libbmdshared_$(PROFILE).so ./examples/linux-user/bmd/libbmdshared.so 
	cp ./examples/linux-user/bmd/libbmdapi_$(PROFILE).so ./examples/linux-user/bmd/libbmdapi.so 
	cp ./examples/linux-user/bmd/libbmdpkgsrc_$(PROFILE).so ./examples/linux-user/bmd/libbmdpkgsrc.so 
	cp ./examples/linux-user/bmd/libbmdshell_$(PROFILE).so ./examples/linux-user/bmd/libbmdshell.so 
	cp ./examples/linux-user/cdk/libcdklibc_$(PROFILE).so ./examples/linux-user/cdk/libcdklibc.so 
	cp ./examples/linux-user/cdk/libcdkshell_$(PROFILE).so ./examples/linux-user/cdk/libcdkshell.so 
	cp ./examples/linux-user/cdk/libcdkshared_$(PROFILE).so ./examples/linux-user/cdk/libcdkshared.so 
	cp ./examples/linux-user/cdk/libcdkmain_$(PROFILE).so ./examples/linux-user/cdk/libcdkmain.so 
	cp ./examples/linux-user/cdk/libcdkpkgsrc_$(PROFILE).so ./examples/linux-user/cdk/libcdkpkgsrc.so 
	cp ./examples/linux-user/cdk/libcdkdsym_$(PROFILE).so ./examples/linux-user/cdk/libcdkdsym.so 
	cp ./examples/linux-user/cdk/libcdksym_$(PROFILE).so ./examples/linux-user/cdk/libcdksym.so 
	cp ./examples/linux-user/phy/libphyutil_$(PROFILE).so ./examples/linux-user/phy/libphyutil.so 
	cp ./examples/linux-user/phy/libphygeneric_$(PROFILE).so ./examples/linux-user/phy/libphygeneric.so 
	cp ./examples/linux-user/phy/libphysym_$(PROFILE).so ./examples/linux-user/phy/libphysym.so 
	cp ./examples/linux-user/phy/libphypkgsrc_$(PROFILE).so ./examples/linux-user/phy/libphypkgsrc.so 



install:
	install -m 755 ./examples/linux-user/bmd/libbmdshared.so $(INSTALL_DIR)/lib/private
	$(STRIP) $(INSTALL_DIR)/lib/private/libbmdshared.so
	install -m 755 ./examples/linux-user/bmd/libbmdapi.so $(INSTALL_DIR)/lib/private
	$(STRIP) $(INSTALL_DIR)/lib/private/libbmdapi.so
	install -m 755 ./examples/linux-user/bmd/libbmdpkgsrc.so $(INSTALL_DIR)/lib/private
	$(STRIP) $(INSTALL_DIR)/lib/private/libbmdpkgsrc.so
	install -m 755 ./examples/linux-user/bmd/libbmdshell.so $(INSTALL_DIR)/lib/private
	$(STRIP) $(INSTALL_DIR)/lib/private/libbmdshell.so
	install -m 755 ./examples/linux-user/cdk/libcdklibc.so $(INSTALL_DIR)/lib/private
	$(STRIP) $(INSTALL_DIR)/lib/private/libcdklibc.so
	install -m 755 ./examples/linux-user/cdk/libcdkshell.so $(INSTALL_DIR)/lib/private
	$(STRIP) $(INSTALL_DIR)/lib/private/libcdkshell.so
	install -m 755 ./examples/linux-user/cdk/libcdkshared.so $(INSTALL_DIR)/lib/private
	$(STRIP) $(INSTALL_DIR)/lib/private/libcdkshared.so
	install -m 755 ./examples/linux-user/cdk/libcdkmain.so $(INSTALL_DIR)/lib/private
	$(STRIP) $(INSTALL_DIR)/lib/private/libcdkmain.so
	install -m 755 ./examples/linux-user/cdk/libcdkpkgsrc.so $(INSTALL_DIR)/lib/private
	$(STRIP) $(INSTALL_DIR)/lib/private/libcdkpkgsrc.so
	install -m 755 ./examples/linux-user/cdk/libcdkdsym.so $(INSTALL_DIR)/lib/private
	$(STRIP) $(INSTALL_DIR)/lib/private/libcdkdsym.so
	install -m 755 ./examples/linux-user/cdk/libcdksym.so $(INSTALL_DIR)/lib/private
	$(STRIP) $(INSTALL_DIR)/lib/private/libcdksym.so
	install -m 755 ./examples/linux-user/phy/libphyutil.so $(INSTALL_DIR)/lib/private
	$(STRIP) $(INSTALL_DIR)/lib/private/libphyutil.so
	install -m 755 ./examples/linux-user/phy/libphygeneric.so $(INSTALL_DIR)/lib/private
	$(STRIP) $(INSTALL_DIR)/lib/private/libphygeneric.so
	install -m 755 ./examples/linux-user/phy/libphysym.so $(INSTALL_DIR)/lib/private
	$(STRIP) $(INSTALL_DIR)/lib/private/libphysym.so
	install -m 755 ./examples/linux-user/phy/libphypkgsrc.so $(INSTALL_DIR)/lib/private
	$(STRIP) $(INSTALL_DIR)/lib/private/libphypkgsrc.so



clean:
	-rm  ./examples/linux-user/bmd/libbmdshared.so  ./examples/linux-user/bmd/libbmdapi.so  ./examples/linux-user/bmd/libbmdpkgsrc.so  ./examples/linux-user/bmd/libbmdshell.so  ./examples/linux-user/cdk/libcdklibc.so  ./examples/linux-user/cdk/libcdkshell.so  ./examples/linux-user/cdk/libcdkshared.so  ./examples/linux-user/cdk/libcdkmain.so  ./examples/linux-user/cdk/libcdkpkgsrc.so  ./examples/linux-user/cdk/libcdkdsym.so  ./examples/linux-user/cdk/libcdksym.so  ./examples/linux-user/phy/libphyutil.so  ./examples/linux-user/phy/libphygeneric.so  ./examples/linux-user/phy/libphysym.so  ./examples/linux-user/phy/libphypkgsrc.so    $(INSTALL_DIR)/lib/private/libbmdshared.so  $(INSTALL_DIR)/lib/private/libbmdapi.so  $(INSTALL_DIR)/lib/private/libbmdpkgsrc.so  $(INSTALL_DIR)/lib/private/libbmdshell.so  $(INSTALL_DIR)/lib/private/libcdklibc.so  $(INSTALL_DIR)/lib/private/libcdkshell.so  $(INSTALL_DIR)/lib/private/libcdkshared.so  $(INSTALL_DIR)/lib/private/libcdkmain.so  $(INSTALL_DIR)/lib/private/libcdkpkgsrc.so  $(INSTALL_DIR)/lib/private/libcdkdsym.so  $(INSTALL_DIR)/lib/private/libcdksym.so  $(INSTALL_DIR)/lib/private/libphyutil.so  $(INSTALL_DIR)/lib/private/libphygeneric.so  $(INSTALL_DIR)/lib/private/libphysym.so  $(INSTALL_DIR)/lib/private/libphypkgsrc.so 
