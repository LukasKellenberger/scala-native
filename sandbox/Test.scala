object Test {
  def main(args: Array[String]): Unit = {
    val longLivingTree = buildTree(25)
    println("build long living tree")
    var i = 0

    var totalDepth = 0
    while (i < 10) {
      val tree = buildTree(24)
      totalDepth += treeDepth(tree)

      i += 1
    }

    println(totalDepth)

  }

  trait Tree
  case class Node(left: Tree, right: Tree) extends Tree
  case object Leaf                         extends Tree

  def buildTree(depth: Int): Tree = depth match {
    case 0 => Leaf
    case n => Node(buildTree(n - 1), buildTree(n - 1))
  }

  def treeDepth(tree: Tree): Int = tree match {
    case Leaf          => 0
    case Node(left, _) => 1 + treeDepth(left)
  }

}
