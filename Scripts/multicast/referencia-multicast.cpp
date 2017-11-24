// referencia-multicast.cpp
// Arquivo de referência da topologia, sem usuários maliciosos, roteamento por multicast.

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/ndnSIM-module.h"

namespace ns3 {

int
main(int argc, char* argv[])
{
  CommandLine cmd;
  cmd.Parse(argc, argv);

  AnnotatedTopologyReader topologyReader("", 1);
  topologyReader.SetFileName("src/ndnSIM/examples/topologies/backbone.txt");
  topologyReader.Read();

  // Install NDN stack on all nodes
  ndn::StackHelper ndnHelper;
  ndnHelper.InstallAll();

  // Choosing forwarding strategy
  ndn::StrategyChoiceHelper::InstallAll("/prefix", "/localhost/nfd/strategy/multicast");

  // Installing global routing interface on all nodes
  ndn::GlobalRoutingHelper ndnGlobalRoutingHelper;
  ndnGlobalRoutingHelper.InstallAll();

  // Getting containers for the consumer/producer
  Ptr<Node> consumers[18] = {Names::Find<Node>("c1"), Names::Find<Node>("c2"),
                            Names::Find<Node>("c3"), Names::Find<Node>("c4"),
                            Names::Find<Node>("c5"), Names::Find<Node>("c6"),
                            Names::Find<Node>("c7"), Names::Find<Node>("c8"),
                            Names::Find<Node>("c9"), Names::Find<Node>("c10"),
                            Names::Find<Node>("c11"), Names::Find<Node>("c12"),
                            Names::Find<Node>("c13"), Names::Find<Node>("c14"),
                            Names::Find<Node>("c15"), Names::Find<Node>("c16"),
                            Names::Find<Node>("c17"), Names::Find<Node>("c18")};
  Ptr<Node> producers[4] = {Names::Find<Node>("p1"), Names::Find<Node>("p2"),
                            Names::Find<Node>("p3"), Names::Find<Node>("p4")};

  for (int i = 0; i < 10; i++) {
    ndn::AppHelper consumerHelper("ns3::ndn::ConsumerCbr");
    consumerHelper.SetAttribute("Frequency", StringValue("100")); // 10 interests a second

    // Each consumer will express unique interests /root/<cname>/<seq-no>
    consumerHelper.SetPrefix("/p1/" + Names::FindName(consumers[i]));
    ApplicationContainer consumer = consumerHelper.Install(consumers[i]);

    consumer.Start(Seconds(i/2));     // start consumers at 0s, 1s, 2s, 3s, 4s and 5s
    consumer.Stop(Seconds(30 - i/2)); // stop consumers at 25s, 26s, 27s, 28s, 29s and 30s
  }

  for (int i = 10; i < 18; i++) {
    ndn::AppHelper consumerHelper("ns3::ndn::ConsumerCbr");
    consumerHelper.SetAttribute("Frequency", StringValue("50")); // 10 interests a second

    // Each consumer will express unique interests /root/<cname>/<seq-no>
    consumerHelper.SetPrefix("/p1/" + Names::FindName(consumers[i]));
    ApplicationContainer consumer = consumerHelper.Install(consumers[i]);

    consumer.Start(Seconds(i/4));     // start consumers at 2s, 3s, 4s and 5s
    consumer.Stop(Seconds(30 - i/4)); // stop consumers at 27s, 28s, 29s and 30s
  }

  for (int i = 0; i < 4; i++) {
    std::string prefix = "/p1";

    //Instala o app nos provedores do conteudo que vao responder conteudos com prefixo /data/
    ndn::AppHelper producerHelper("ns3::ndn::Producer");
    producerHelper.SetAttribute("PayloadSize", StringValue("1024"));
    producerHelper.SetPrefix(prefix);
    ApplicationContainer producer = producerHelper.Install(producers[i]);
    ndnGlobalRoutingHelper.AddOrigins(prefix, producers[i]);
  }

  // Calculate and install FIBs
  ndn::GlobalRoutingHelper::CalculateRoutes();

  Simulator::Stop(Seconds(40.0));

  ndn::L3RateTracer::InstallAll("rate-trace.txt", Seconds(0.5));
  L2RateTracer::InstallAll("drop-trace.txt", Seconds(0.5));

  Simulator::Run();
  Simulator::Destroy();

  return 0;
}

} // namespace ns3

int
main(int argc, char* argv[])
{
  return ns3::main(argc, argv);
}
