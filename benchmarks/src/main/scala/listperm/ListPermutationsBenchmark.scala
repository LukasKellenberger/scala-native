package listperm

import benchmarks.{
  Benchmark,
  BenchmarkCompanion,
  BenchmarkRunningTime,
  VeryLongRunningTime
}

object ListPermutationsBenchmark extends BenchmarkCompanion[Int] {
  override def apply(): Benchmark[Int] = new ListPermutationsBenchmark()
}

class ListPermutationsBenchmark extends benchmarks.Benchmark[Int] {
  val size = 8

  override val runningTime: BenchmarkRunningTime = VeryLongRunningTime

  override def run(): Int = {
    val permIter = (0 until size).toList.permutations

    var count = 0
    while (permIter.hasNext) {
      permIter.next()
      count += 1
    }
    count
  }

  override def check(value: Int): Boolean = {
    value == factorial(size)
  }

  private def factorial(i: Int): Int = {
    var n    = i
    var fact = 1
    while (n > 0) {
      fact *= n
      n -= 1
    }
    fact
  }
}
