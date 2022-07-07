MODULES=kernel loader boot

@all:
	echo -e ""

config:
	@ for module in $(MODULES); do make -C $$module config; done	

clean:
	@ for module in $(MODULES); do make -C $$module clean; done	

compile:
	@ for module in $(MODULES); do make -C $$module; done	
