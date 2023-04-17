noinst_LTLIBRARIES = libWARPBeacon.la

libexec_PROGRAMS = warp-beacon

libWARPBeacon_la_SOURCES = Mux.cc Demux.cc MuxDelegate.cc Packet.cc Packets.cc

warp_beacon_SOURCES = Beacon.cc \
	DebugMux.cc MuxProxy.cc TCPListener.cc TCPListener.hh

warp_beacon_LDADD = ../Core/libWARPCore.la libWARPBeacon.la

