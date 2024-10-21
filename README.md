# reference-system-autoware

With the distributed development of ROS across many different organizations it is sometimes hard to benchmark and concretely show how a certain change to a certain system improves or reduces the performance of that system. For example did a change from one executor to another actually reduce the CPU or was it something else entirely?

> 随着 ROS 在许多不同组织中的分布式开发，有时很难对某个系统进行基准测试和具体展示对某个系统的特定更改如何改善或降低该系统的性能。例如，从一个执行程序到另一个执行程序的更改实际上是减少了 CPU，还是完全是其他原因？

In order to try and address this problem we at [Apex.AI](https://apex.ai) would like to propose a definition of a [_reference system_](#reference-system) that simulates a real world scenario - in this case Autoware.Auto and its lidar data pipeline - that can be repeated no matter the underlying change of any piece of the full stack (i.e. executor, DDS or even RMW).

> 为了尝试解决这个问题，我们 [Apex.AI](https://apex.ai) 希望提出一个参考系统的定义，该[_参考系统_](#reference-system)模拟真实世界场景 - 在本例中为 Autoware.Auto 及其激光雷达数据管道 - 无论完整堆栈的任何部分（即执行程序、DDS 甚至 RMW）的底层更改如何，都可以重复。

![Node graph of reference-system-autoware](content/img/dotgraph_autoware.svg)

Future reference systems could be proposed that are more complex using the same basic node building blocks developed for this first scenario.

> 可以使用为第一种情况开发的相同基本节点构建块，提出更复杂的未来参考系统。

## Reference System

A _reference system_ is defined by:

> *参考系*由以下各项定义：

- A [platform](#supported-platforms) is defined by:
  [平台](#supported-platforms)由以下各项定义：
  - Hardware (e.g. an off-the-shelf single-board computer, embedded ECU, etc.)
    硬件（例如现成的单板计算机、嵌入式 ECU 等）
    - if there are multiple configurations available for such hardware, ensure it is specified  
      如果此类硬件有多个配置可用，请确保已指定
  - Operating System (OS) like RT linux, QNX, etc. along with any special configurations made  
    操作系统 （OS），如 RT linux、QNX 等，以及所做的任何特殊配置
- for simplicity and ease of benchmarking, **all nodes must run on a single process**
  为了简单和易于进行基准测试，**所有节点都必须在单个进程上运行**
- a fixed number of nodes  
  固定数量的节点
  - each node with:
    每个节点具有：
    - a fixed number of publishers and subscribers  
      固定数量的发布者和订阅者
    - a fixed _processing time_ or a fixed _publishing rate_  
      固定*的处理时间*或固定*的发布速率*
- a fixed _message type_ of fixed size to be used for every _node_.
  用于每个*节点*的固定大小的固定*消息类型*。

With these defined attributes the _reference system_ can be replicated across many different possible configurations to be used to benchmark each configuration against the other in a reliable and fair manner.

> 通过这些定义的属性，可以在许多不同的可能配置中复制*参考系统*，以可靠和公平的方式将每个配置与其他配置进行基准测试。

With this approach [unit tests](#unit-testing) can also be defined to reliably confirm if a given _reference system_ meets the requirements.

> 使用这种方法，还可以定义[单元测试](#unit-testing)，以可靠地确认给定*的参考系统*是否满足要求。

## Supported Platforms

To enable as many people as possible to replicate this reference system, the platform(s) were chosen to be easily accessible (inexpensive, high volume), have lots of documentation, large community use and will be supported well into the future.

> 为了让尽可能多的人能够复制这个参考系统，我们选择了易于访问（便宜、大容量）、有大量文档、大量社区使用并将在未来得到支持。

Platforms were not chosen for performance of the reference system - we know we could run “faster” with a more powerful CPU or GPU but then it would be harder for others to validate findings and test their own configurations. Accessibility is the key here and will be considered if more platforms want to be added to this benchmark list.

> 选择平台并不是为了参考系统的性能 - 我们知道，使用更强大的 CPU 或 GPU 可以“更快”运行，但其他人将更难验证结果和测试自己的配置。可访问性是这里的关键，如果更多平台想要添加到此基准列表中，我们将考虑可访问性。

**Platforms:**

- [Raspberry Pi 4B](https://www.raspberrypi.org/products/raspberry-pi-4-model-b/):
  [树莓派 4B](https://www.raspberrypi.org/products/raspberry-pi-4-model-b/)：
  - 4 GB RAM version is the assumed default  
    4 GB RAM 版本是假定的默认值
    - other versions could also be tested / added by the community  
      社区也可以测试/添加其他版本
  - [real-time linux kernel  
    实时 Linux 内核](https://github.com/ros-realtime/rt-kernel-docker-builder)

\_Note: create an [issue](https://github.com/ros-realtime/reference-system-autoware/issues/) to add more platforms to the list, keeping in mind the above criteria

> 注意：创建一个 [issue](https://github.com/ros-realtime/reference-system-autoware/issues/) 以将更多平台添加到列表中，同时牢记上述标准\_

## Concept Overview

Rather than trying to write code to cover all potential variations of executors, APIs, and future features we cannot even imagine today we have chosen instead to define what we call a “reference system” based on part of a real-world system, [Autoware.Auto](https://www.autoware.org/autoware-auto).

> 我们没有尝试编写代码来涵盖我们今天甚至无法想象的执行程序、API 和未来功能的所有潜在变体，而是选择基于真实系统的一部分 [Autoware.Auto](https://www.autoware.org/autoware-auto) 定义我们所谓的“参考系统”。

The above node graph can be boiled down to only a handful of node "types" that are replicated to make this complex system:

> 上面的节点图可以归结为少数几个节点 “类型” 被复制以构成这个复杂的系统：

**Node Types:**

1. [**Sensor Node/传感器节点**](reference_system/include/reference_system/nodes/rclcpp/sensor.hpp)
   - input node to system  
     系统输入节点
   - publishes message cyclically at some fixed frequency  
     以某个固定频率循环发布消息
2. [**Transformer Node/Transformer 节点**](reference_system/include/reference_system/nodes/rclcpp/transformer.hpp)
   - one subscriber, one publisher  
     一个订阅者，一个发布者
   - starts processing for N milliseconds after a message is received  
     在收到消息后 N 毫秒内开始处理
   - publishes message after processing is complete  
     处理完成后发布消息
3. [**Fusion Node/Fusion 节点**](reference_system/include/reference_system/nodes/rclcpp/fusion.hpp)
   - two subscribers, one publisher  
     两个订阅者，一个发布者
   - starts processing for N milliseconds after a message is received **from all** subscriptions  
     从**所有**订阅收到消息后开始处理 N 毫秒
   - publishes message after processing is complete  
     处理完成后发布消息
4. [**Reactor Node/Reactor 节点**](reference_system/include/reference_system/nodes/rclcpp/reactor.hpp)
   - 0..N subscribers, one publisher  
     0..N 个订阅者，一个发布者
   - cyclically publishes at least one message or one for every received message  
     循环发布至少一条消息，或为每条收到的消息发布一条消息
5. [**Command Node/命令节点**](reference_system/include/reference_system/nodes/rclcpp/command.hpp)
   - prints output stats everytime a message is received  
     每次收到消息时打印输出统计信息

These basic building-block nodes can be mixed-and-matched to create quite complex systems that replicate real-world scenarios to benchmark different configurations against each other.

> 这些基本的构建块节点可以混合和匹配，以创建相当复杂的系统，这些系统复制真实场景，以将不同的配置相互进行基准测试。

## Reference Systems Overview

The first reference system benchmark proposed is based on the _Autoware.Auto_ lidar data pipeline as stated above and shown in the node graph image above as well.
提出的第一个参考系统基准测试基于如上所述的 _Autoware.Auto_ 激光雷达数据管道，也如上面的节点图图像所示。

1. [**Reference System Autoware.Auto/参考系统 Autoware.Auto**](reference_system/reference_system_autoware.md)
   - ROS2:
     - Executors:
       - Default:
         - [Single Threaded](reference_system/src/ros2/executor/autoware_default_singlethreaded.cpp)
         - [Static Singe Threaded](reference_system/src/ros2/executor/autoware_default_staticsinglethreaded.cpp)
         - [Multithreaded](reference_system/src/ros2/executor/autoware_default_multithreaded.cpp)

Results below show various characteristics of the same simulated system (Autoware.Auto).

> 下面的结果显示了同一模拟系统 （Autoware.Auto） 的各种特征。

To add your own executor / middleware / configuration to the list above follow the _Contributing_ section below.

> 要将您自己的 executor / middleware / configuration 添加到上面的列表中，请按照下面的 _Contributing_ 部分进行操作。

## Benchmark Results

Results will be added to different tagged releases along with the specific configurations ran during the tests.

> 结果将与测试期间运行的特定配置一起添加到不同的标记版本中。

## Unit Testing

Unit tests can be written for the _reference system_ to check if all nodes, topics and other requirements are met before accepting test results.

> 可以为*参考系统*编写单元测试，以在接受测试结果之前检查是否满足所有节点、主题和其他要求。

Tests are provided to automatically generate results for you by running `colcon test` on a supported platform above.

> 通过在上述支持的平台上运行 `colcon test` 来提供测试以自动生成结果。

To run the test, simply run the following command from your workspace:

> 要运行测试，只需从您的工作区运行以下命令：

```bash
colcon test  --packages-up-to reference_system_autoware
```

Alternatively if for some reason you do not or cannot use `colcon` the tests are simple `gtests` that can be ported and ran on any configuration.

> 或者，如果由于某种原因您不使用或无法使用 `colcon`，则测试是简单的 `gtest`，可以在任何配置上移植和运行。

## Contributing

If you see a missing configuration on the list above that you would like to see benchmarked against please follow the steps below to request it to be added.

> 如果您在上面的列表中看到缺少的配置，并且您希望对其进行基准测试，请按照以下步骤请求添加该配置。

- look over the open / closed [issues](https://github.com/ros-realtime/reference-system-autoware/issues/) to make sure there isn't already an open ticket for the configuration you are looking for create `include/reference_system/MY_EXECUTOR_NAME_nodes`

## Howto Implement Your Custom Executor

1. Read over [the above documentation](#concept-overview) on the base node types
2. Review the base [`rclcpp nodes`](include/reference_system/nodes/rclcpp) that are provided and determine if your executor can use them
3. If you cannot, implment your own version of each base node type and place the source in [`include/reference_system/nodes`](include/reference_system/nodes)
4. Add your new nodes as a seperate `node system` in [`include/reference_system/system/systems.hpp`](include/reference_system/system/systems.hpp)
5. Copy one of the provided example `.cpp` files from the [`src/ros2/executor`](src/ros2/executor) directory and replace the `create_autoware_nodes` template type with your new `node system` which should be in the `system/systems.hpp` file already included
6. Add new `.cpp` source file as a new executable in the `CMakelist.txt`
7. Add new executable to test wtihin the `CMakelist.txt`
8. Build and run tests!

- 阅读上述有关基本节点类型的[文档](#concept-overview)
- 查看提供的基本 [`rclcpp 节点`](include/reference_system/nodes/rclcpp)，并确定您的执行程序是否可以使用它们
- 如果不能，请实现每个基本节点类型的您自己的版本，并将源放在 [`include/reference_system/nodes`](include/reference_system/nodes)
- 将新节点添加为单独的`节点系统` [`include/reference_system/system/systems.hpp`](include/reference_system/system/systems.hpp)
- 从 [`src/ros2/executor`](src/ros2/executor) 目录中复制提供的示例 `.cpp` 文件之一，并将 `create_autoware_nodes` 模板类型替换为您的新`节点系统，该节点系统`应位于已包含的 `system/systems.hpp` 文件中
- 在 `CMakelist.txt` 中将新的 `.cpp` 源文件添加为新的可执行文件
- 添加新的可执行文件以测试 `CMakelist.txt`
- 构建并运行测试！
