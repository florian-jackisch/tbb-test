#include <functional>
#include <iostream>
#include <tuple>

#include "tbb/flow_graph.h"

template <typename Input, typename Output, typename... Args>
class FunctionNode
    : public tbb::flow::composite_node<std::tuple<Input>, std::tuple<Output>> {
  using BaseType =
      typename tbb::flow::composite_node<std::tuple<Input>, std::tuple<Output>>;

public:
  FunctionNode(tbb::flow::graph &g, size_t concurrency,
               std::function<Output(const Input &)> &&f)
      : BaseType{g}, func_{std::move(f)}, node_{g, concurrency, func_} {
    typename BaseType::input_ports_type inputPorts{node_};
    typename BaseType::output_ports_type outputPorts{node_};
    BaseType::set_external_ports(inputPorts, outputPorts);
  }

private:
  std::function<Output(const Input &)> func_;
  tbb::flow::function_node<Input, Output, Args...> node_;
};

int main() {
  using namespace tbb::flow;
  graph g;

  FunctionNode<int, int> plusOneNode{g, 1, [](int i) { return i + 1; }};

  function_node<int> printNode{
      g, 1, [](int i) { std::cout << "printer: " << i << '\n'; }};

  make_edge(output_port<0>(plusOneNode), printNode);

  input_port<0>(plusOneNode).try_put(1);
  input_port<0>(plusOneNode).try_put(2);
  input_port<0>(plusOneNode).try_put(3);
  input_port<0>(plusOneNode).try_put(4);
  input_port<0>(plusOneNode).try_put(5);

  g.wait_for_all();
}
