
ap.srcs += $(SRC_BOOZ)/booz_gps.c
ap.CFLAGS += -DBOOZ_GPS_TYPE_H=\"gps/booz_gps_ubx.h\"
ap.srcs += $(SRC_BOOZ)/gps/booz_gps_ubx.c

ap.CFLAGS += -DUSE_$(GPS_PORT) -D$(GPS_PORT)_BAUD=$(GPS_BAUD)
ap.CFLAGS += -DUSE_GPS -DGPS_LINK=$(GPS_PORT)

ifneq ($(GPS_LED),none)
  ap.CFLAGS += -DGPS_LED=$(GPS_LED)
endif

ifeq ($(ARCH), lpc21)
ap.CFLAGS += -D$(GPS_PORT)_VIC_SLOT=5
endif

sim.CFLAGS += -DUSE_GPS
sim.srcs += $(SRC_BOOZ)/booz_gps.c
