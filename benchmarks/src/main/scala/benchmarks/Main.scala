package benchmarks

object Main {
  def main(args: Array[String]): Unit = {

    println(TextFormat.show(Seq(Benchmark.readResult(args(0)))))
  }
}
