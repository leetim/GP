# type Type = Int
class SomeClass
  var $1: Int
  var $some: Float
  # $p.p
end
var $a: SomeClass
# var $b: String

def foo $arg: Int, $arg2: String->Int
  return $arg + 2
end

do
  var $b: Int
  $a.$some = 2.0 + 2
  # if 5
  #   $b = 0
  # end
  # for i in 1..3
  #   $b = $b + 1
  # end
  while ($b != 3)
    $b = $b - 2
  end
  # $a.$1? = foo 1, 2
end

# do
#   # var $a: Int = 10
#   # var $b: Float = 15
#   $a = $b.$c
# end
