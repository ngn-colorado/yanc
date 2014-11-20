#!/usr/bin/env python2

from __future__ import print_function
import dbus, dbus.mainloop.glib, dbus.service
import gobject
import dpkt.ethernet
import os, os.path

ROOT	  = "/net"
DBUS_PATH = "unix:path=/tmp/yanc_bus"

OFPP_MAX        = 0xff00
OFPP_FLOOD      = 0xfffb
OFPP_CONTROLLER = 0xfffd

OFPFW_IN_PORT  = 1 << 0
OFPFW_DL_VLAN  = 1 << 1
OFPFW_DL_SRC   = 1 << 2
OFPFW_DL_DST   = 1 << 3
OFPFW_DL_TYPE  = 1 << 4
OFPFW_NW_PROTO = 1 << 5
OFPFW_TP_SRC   = 1 << 6
OFPFW_TP_DST   = 1 << 7
OFPFW_TP_DST   = 1 << 7
OFPFW_DL_VLAN_PCP = 1 << 20
OFPFW_NW_TOS   = 1 << 21
OFPFW_ALL      = (1 << 22) - 1

class YCBench(object):

	def __init__(self, root="/net", dbus_path="unix:path=/tmp/yanc_bus"):

		dbus.mainloop.glib.DBusGMainLoop(set_as_default=True)
		self.bus  = dbus.bus.BusConnection(address_or_type=dbus_path)
		self.loop = gobject.MainLoop()

		self.bus.add_signal_receiver(self.handle_new_switch, signal_name = "NewSwitch")
		self.bus.add_signal_receiver(self.handle_packet_in,  signal_name = "PacketIn")
		#self.bus.add_signal_receiver(self.handle_all,
		#		sender_keyword      = "sender",
		#		destination_keyword = "destination",
		#		interface_keyword   = "interface",
		#		member_keyword      = "member",
		#		path_keyword        = "path",
		#		message_keyword     = "message",
		#		)

		self.flow_counter = 0

		self.sig_sender = YSignalSender(self.bus)

	def run(self):

		self.loop.run()

	def handle_new_switch(self, path):

		obj = YSwitch(path)
		print("NewSwitch:", obj)

	def handle_packet_in(self, path):

		obj = YPacketIn(path)
		print("PacketIn: ", obj)

		sw_path = obj.path.split("/")[:4]
		flow_path = "/".join(sw_path + ["flows", str(self.flow_counter)])
		self.flow_counter += 1
		flow = YFlowMod(flow_path, mk=True)
		flow.create_from_packet_in(obj)
		rel_path = flow.path.split("/")
		del rel_path[1]
		self.sig_sender.NewFlow("/".join(rel_path))

	def handle_all(self, *args, **kwargs):

		print("Signal:", *args)
		for k,v in kwargs.iteritems():
			print("\t",k,"::", v)

class YSignalSender(dbus.service.Object):

	def __init__(self, bus):

		dbus.service.Object.__init__(self, bus, "/")

	@dbus.service.signal(dbus_interface="edu.colorado.cs.ngn.yanc", signature="s")
	def NewFlow(self, path):
		print("NewFlow:  ", path)


class YancObject(object):

	def __init__(self, path, mk=False):

		self.path    = path

		if mk and not os.path.exists(path):
			os.mkdir(path)
			self.created = True
		else:
			self.created = False
			self.load()
	
	def __str__(self):

		return self.path

	def __getitem__(self, key):

		with open(self.path + "/" + key, "r") as fd:
			val = fd.read().strip()
		return val

	def __setitem__(self, key, val):

		with open(self.path + "/" + key, "w") as fd:
			fd.write(str(val))

class YSwitch(YancObject):

	def load(self):

		self.datapath_id = self["datapath_id"]
		self.n_buffers   = int(self["n_buffers"])
		self.n_tables    = int(self["n_tables"])

	def __str__(self):

		return "%s[id=%s]" % \
			(self.path, self.datapath_id)

class YPacketIn(YancObject):

	def load(self):

		self.buffer_id = int(self["buffer_id"])
		self.in_port   = int(self["in_port"])
		self.total_len = int(self["total_len"])

		data = self["data"]
		eth  = dpkt.ethernet.Ethernet(data)

		self.eth_src = mac_str(eth.src)
		self.eth_dst = mac_str(eth.dst)

	def __str__(self):

		return "%s[bufid=%d port=%d len=%d eth_src=%s eth_dst=%s]" % \
			(self.path, self.buffer_id, self.in_port, self.total_len,
					self.eth_src, self.eth_dst)

class YFlowMod(YancObject):

	def create_from_packet_in(self, pktin):

		self["match/wildcards"] = OFPFW_ALL - OFPFW_DL_SRC - OFPFW_DL_DST
		self["match/dl_dst"]    = pktin.eth_dst
		self["match/dl_src"]    = pktin.eth_src
		self["buffer_id"]       = pktin.buffer_id

	def __str__(self):
		return "%s[bufid=%s]" % \
			(self.path, self["buffer_id"])


def mac_str(mac):
	return ':'.join("%02x" % ord(b) for b in mac)

if __name__ == "__main__":

	YCBench().run()

# vim: set noet ts=8 sw=8 sts=0 :
