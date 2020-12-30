/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/**
 * ns-3 files, that influenced this file:
 * - examples/wireless/wifi-adhoc.cc
 */

#include "ns3/gnuplot.h"
#include "ns3/core-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/mobility-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/yans-wifi-helper.h"
#include "ns3/ssid.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("PracticalAssignment");

// from examples/wireless/wifi-adhoc.cc, let's see how much i will use of this code and whether i forget to delete the rest :)

void SetPosition (Ptr<Node> node, Vector position)
{
  Ptr<MobilityModel> mobility = node->GetObject<MobilityModel> ();
  mobility->SetPosition (position);
}

Vector GetPosition (Ptr<Node> node)
{
  Ptr<MobilityModel> mobility = node->GetObject<MobilityModel> ();
  return mobility->GetPosition ();
}

void SendPacket(Ptr<Socket> socket, double count) {
	if(count == 0) {
		return;
	}
	socket->Send(Create<Packet>());
	Simulator::Schedule(Seconds(1.0), &SendPacket, socket, count-1);
}

void Step(Ptr<Node> node, double advancement, double packetsPerStep, Ptr<Socket> socket)
{
	Vector pos = GetPosition (node);
	pos.x += advancement;
	SetPosition (node, pos);
	Simulator::Schedule (Seconds (packetsPerStep), &Step, node, advancement, packetsPerStep, socket);
	Simulator::Schedule (Seconds(0.5), &SendPacket, socket, packetsPerStep);
}

void ReceivePacket (Ptr<Socket> socket)
{
  Ptr<Packet> packet;
  while ((packet = socket->Recv ()))
    {
    }
}

Ptr<Socket> SetupPacketReceive (Ptr<Node> node)
{
  TypeId tid = TypeId::LookupByName ("ns3::PacketSocketFactory");
  Ptr<Socket> sink = Socket::CreateSocket (node, tid);
  sink->Bind ();
  sink->SetRecvCallback (MakeCallback (&ReceivePacket));
  return sink;
}

int main(int argc, char *argv[]) {
	char lossModel = 'l';
	double stepWidth = 1.0;
	double duration = 0.0;
	double packetsPerStep = 1.0;
	double startPos = 0.0;

	CommandLine cmd(__FILE__);

	cmd.AddValue ("lossMod", "LossModel to be used: f = FriisPropagationModel, t = TwoRayGroundPropagationLossModel, l = LogDistancePropagationLossModel, s = self implemented TwoRay, j = JakesPropagationLossModel, c = combined Jakes and LogDist", lossModel);
	cmd.AddValue ("stepWidth", "(x-)position advancement of station B per step in m", stepWidth);
	cmd.AddValue ("duration", "duration of this whole experiment", duration);
	cmd.AddValue ("packetsPerStep", "number of packets that A sends per step", packetsPerStep);
	cmd.AddValue ("startPos", "start (x-)position of station B in m (default 0.0)", startPos);

	cmd.Parse(argc, argv);

	NodeContainer c;
	c.Create(2);

	WifiHelper wifi;

	YansWifiChannelHelper wifiChannel = YansWifiChannelHelper();
	wifiChannel.SetPropagationDelay ("ns3::ConstantSpeedPropagationDelayModel");

	if(lossModel == 'f') {
		wifiChannel.AddPropagationLoss("ns3::FriisPropagationLossModel",
				"Frequency", DoubleValue(2.4e9));
	} else if(lossModel == 't') {
		wifiChannel.AddPropagationLoss("ns3::TwoRayGroundPropagationLossModel",
				"Frequency", DoubleValue(2.4e9),
				"HeightAboveZ", DoubleValue(1));
	} else if(lossModel == 'l') {
		wifiChannel.AddPropagationLoss("ns3::LogDistancePropagationLossModel");
	} else if(lossModel == 's') {
		// TODO: task 4:
		wifiChannel.AddPropagationLoss("ns3::NewTwoRayGroundPropagationLossModel",
				"MinDistance", DoubleValue(0.5),
				"Wavelength", DoubleValue(0.125),
				"HeightAboveZ", DoubleValue(1.0));
	} else if(lossModel == 'j') {
		// task 2.2
		wifiChannel.AddPropagationLoss("ns3::JakesPropagationLossModel");
	} else if(lossModel == 'c') {
		// task 3.1 and 3.2
		wifiChannel.AddPropagationLoss("ns3::TwoRayGroundPropagationLossModel",
				"Frequency", DoubleValue(2.4e9),
				"HeightAboveZ", DoubleValue(1));
		wifiChannel.AddPropagationLoss("ns3::JakesPropagationLossModel");
	}

	LogComponentEnableAll (LogLevel (LOG_PREFIX_TIME));
	LogComponentEnable("YansWifiChannel", LOG_LEVEL_DEBUG);
	YansWifiPhyHelper wifiPhy = YansWifiPhyHelper::Default();
	wifiPhy.Set("RxGain", DoubleValue(-2));
	wifiPhy.Set("TxGain", DoubleValue(1));
	wifiPhy.SetChannel(wifiChannel.Create());

	WifiMacHelper wifiMac;
	wifiMac.SetType("ns3::AdhocWifiMac");

	NetDeviceContainer devices = wifi.Install (wifiPhy, wifiMac, c);

	//PacketSocketHelper packetSocket;
	//packetSocket.Install (c);

	MobilityHelper mobility;
	Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();
	positionAlloc->Add (Vector (0.0, 0.0, 0.0));
	positionAlloc->Add (Vector (startPos, 0.0, 0.0));
	mobility.SetPositionAllocator (positionAlloc);
	mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");

	mobility.Install (c);

	InternetStackHelper internet;
	internet.Install(c);

	Ipv4AddressHelper ipv4;
	NS_LOG_INFO ("Assign IP Addresses.");
	ipv4.SetBase ("10.1.1.0", "255.255.255.0");
	Ipv4InterfaceContainer i = ipv4.Assign (devices);

	TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");

	Ptr<Socket> recvSink = Socket::CreateSocket (c.Get (1), tid);
	InetSocketAddress local = InetSocketAddress (Ipv4Address::GetAny (), 80);
	recvSink->Bind (local);
	recvSink->SetRecvCallback (MakeCallback (&ReceivePacket));

	Ptr<Socket> source = Socket::CreateSocket (c.Get (0), tid);
	InetSocketAddress remote = InetSocketAddress (Ipv4Address ("255.255.255.255"), 80);
	source->SetAllowBroadcast (true);
	source->Connect (remote);

	// Tracing
	wifiPhy.EnablePcap ("pa", devices, true);


	// TODO: stationary staA
	// 2.1, 3.1, 3.2: move staB in 1m (5m 3.2) steps to 350m, 1 (100 3.2) packets per step
	// 2.2: stationary staB 10m from staA, 1h
	// always: send one packet per sec from staA to staB

	Simulator::Schedule (Seconds (1.0), &SendPacket, source, packetsPerStep);
	Simulator::Schedule (Seconds (0.5 + packetsPerStep), &Step, c.Get (1), stepWidth, packetsPerStep, source);

	Simulator::Stop(Seconds(0.5 + duration));
	Simulator::Run ();

	Simulator::Destroy ();
}
