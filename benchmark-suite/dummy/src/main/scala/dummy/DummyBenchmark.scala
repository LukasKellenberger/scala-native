package dummy
import benchmarks._

object DummyBenchmark {
  def main(args: Array[String]): Unit = {
    Benchmark.run(new DummyBenchmark(), args)
  }
}

class DummyBenchmark extends Benchmark[Int] {
  override def run(): Int = (1 to 1000).sum

  override def check(t: Int): Boolean = t == 500500
}