noinst_LTLIBRARIES = libWARP-Beacon.la

libexec_PROGRAMS = warp-beacon

libWARP_Beacon_la_SOURCES = Mux.cc Demux.cc MuxDelegate.cc Packet.cc Packets.cc

warp_beacon_SOURCES = Beacon.cc \
	DebugMux.cc MuxProxy.cc TCPListener.cc TCPListener.hh

warp_beacon_LDADD = ../Flux/libWARP-Flux.la libWARP-Beacon.la

