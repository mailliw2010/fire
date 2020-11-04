TEMPLATE = subdirs   # set subdirs: multi projects; set app: single projects

# set subdirs' dir
SUBDIRS += $$PWD/FireService/FireService.pro\
           $$PWD/FireClient/FireClient.pro
          # $$PWD/FireMCCB/FireMCCB.pro

# set ordered to make it by ordered
CONFIG += ordered
