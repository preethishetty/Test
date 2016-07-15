#include <iostream>
   
    #include "ns3/core-module.h"
   #include "ns3/network-module.h"
   #include "ns3/internet-module.h"
  #include "ns3/point-to-point-module.h"
   #include "ns3/netanim-module.h"
   #include "ns3/applications-module.h"
   #include "ns3/point-to-point-layout-module.h"
    
    using namespace ns3;
   
 int main (int argc, char *argv[])
  {
     Config::SetDefault ("ns3::OnOffApplication::PacketSize", UintegerValue (1460));
     Config::SetDefault ("ns3::OnOffApplication::DataRate", StringValue ("500Kb/s"));
  
    uint32_t    nLeftLeaf = 3;
     uint32_t    nRightLeaf = 3;
     uint32_t    nLeaf = 0; 
      std::string animFile = "dumbbell-animation.xml" ;
    CommandLine cmd;
  cmd.AddValue ("nLeftLeaf", "Number of left side leaf nodes", nLeftLeaf);
    cmd.AddValue ("nRightLeaf","Number of right side leaf nodes", nRightLeaf);
   cmd.AddValue ("nLeaf",     "Number of left and right side leaf nodes", nLeaf);
    cmd.AddValue ("animFile",  "File Name for Animation Output", animFile);
    
   cmd.Parse (argc,argv);
     if (nLeaf > 0)
     {
       nLeftLeaf = nLeaf;
       nRightLeaf = nLeaf;
     }
   
  
     PointToPointHelper pointToPointRouter;
    pointToPointRouter.SetDeviceAttribute  ("DataRate", StringValue ("10Mbps"));
    pointToPointRouter.SetChannelAttribute ("Delay", StringValue ("10ms"));
     PointToPointHelper pointToPointLeaf;
     pointToPointLeaf.SetDeviceAttribute    ("DataRate", StringValue ("10Mbps"));
     pointToPointLeaf.SetChannelAttribute   ("Delay", StringValue ("10ms"));
 
     PointToPointDumbbellHelper d (nLeftLeaf, pointToPointLeaf,
                                nRightLeaf, pointToPointLeaf,
                                 pointToPointRouter);
  
    
   InternetStackHelper stack;
     d.InstallStack (stack);
  
     
    d.AssignIpv4Addresses (Ipv4AddressHelper ("10.1.1.0", "255.255.255.0"),
                          Ipv4AddressHelper ("10.2.1.0", "255.255.255.0"),
                         Ipv4AddressHelper ("10.3.1.0", "255.255.255.0"));
   
     
      OnOffHelper clientHelper ("ns3::UdpSocketFactory", Address ());
     clientHelper.SetAttribute ("OnTime", StringValue ("ns3::UniformRandomVariable"));
    clientHelper.SetAttribute ("OffTime", StringValue ("ns3::UniformRandomVariable"));
    ApplicationContainer clientApps;
   
     for (uint32_t i = 1; i < d.RightCount (); ++i)
      {
       
        AddressValue remoteAddress (InetSocketAddress (d.GetLeftIpv4Address (i), 1000));
         clientHelper.SetAttribute ("Remote", remoteAddress);
          clientApps.Add (clientHelper.Install (d.GetRight (i)));
       }
 
     clientApps.Start (Seconds (1.0));
      clientApps.Stop (Seconds (50.0));
   
     
      d.BoundingBox (1, 1, 100, 100);
   
    
   AnimationInterface anim (animFile);
   
    Ipv4GlobalRoutingHelper::PopulateRoutingTables ();



   
    Simulator::Run ();
    std::cout << "Animation Trace file created:" << animFile.c_str ()<< std::endl;
  Simulator::Destroy ();
   return 0;
  }

    
