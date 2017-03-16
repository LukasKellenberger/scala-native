//import scala.scalanative.runtime.GC

object Test {
  def main(args: Array[String]): Unit = {
    System.out.println("hello")
    /*var l: List = Nil
    var i       = 0;
    while (i < 100) {
      //if(i%1000 == 0) GC.collect()
      l = Cons(i.toString, l);
      i += 1;
    }
    println("size " + l.size);*/
  }

  /*object List {
    def size(l: List): Int = {
      var s    = 0;
      var list = l
      while (!list.isInstanceOf[Nil.type]) {
        list = list.asInstanceOf[Cons].next
        s += 1;
      }
      return s
    }
  }

  trait List {
    def size: Int = this match {
      case Nil        => GC.collect(); 0
      case Cons(_, n) => 1 + n.size
    }
  }
  case object Nil                        extends List
  case class Cons(i: String, next: List) extends List*/
}
