# invoke SourceDir generated makefile for app.pe66
app.pe66: .libraries,app.pe66
.libraries,app.pe66: package/cfg/app_pe66.xdl
	$(MAKE) -f C:\Users\tupers\workspace_v6_0\edma_test/src/makefile.libs

clean::
	$(MAKE) -f C:\Users\tupers\workspace_v6_0\edma_test/src/makefile.libs clean

