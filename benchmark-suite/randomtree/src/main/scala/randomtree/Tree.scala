package randomtree

import java.util.Random

import benchmarks.Benchmark

object RandomTree {
  def main(args: Array[String]): Unit = {
    Benchmark.run(new RandomTree(), args)
  }
}

class RandomTree extends Benchmark[Unit] {
  override def run(): Unit = {
    val maxChildren = 12
    val randomGeneratorSeed = 0
    val updateThreshold = 0.01
    val nbPasses = 5

    new Tree(maxChildren, new Random(randomGeneratorSeed), updateThreshold, nbPasses).run()
  }

  override def check(t: Unit): Boolean = true
}



class Tree(maxChildren: Int, rand: Random, updateThreshold: Double, nbPasses: Int) {
  case class Node(depth: Int, children: Seq[Node])

  def random(depth: Int): Int = {
    (Math.pow(rand.nextDouble(), depth) * maxChildren).toInt
  }

  def build(depth: Int): Node = {
    val childrenCount = random(depth)

    val children = for(i <- 0 until childrenCount) yield {
      build(depth + 1)
    }
    Node(depth, children)
  }

  def run(): Unit = {
    (0 until nbPasses).foldLeft(build(0)){ case (node, _) => traverse(node) }
  }

  def traverse(node: Node): Node = {
    val children = node.children.map { child =>
      if(rand.nextDouble() > updateThreshold) {
        build(child.depth)
      } else {
        traverse(child)
      }
    }
    Node(node.depth, children)
  }
}

