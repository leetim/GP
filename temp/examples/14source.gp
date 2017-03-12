package main

use os

def cmd $ind1 Int, $ind2 Int
  os.println "? %d %d", $ind1, $ind2
  $t Char
  os.readln $t
  return $t == '<'
end

do
  $n Int
  os.readln $n
  for $z in 0..$n
    @top Int[]
    @bottom Int[]
    $T Int
    os.readln $T
    if $T == 1
      os.println "! 1 1"
      continue
    end
    for $i in 1..$T:2
      if cmd $i + 1, i
        @top.push_back $i
        @bottom.push_back $i + 1
      else
        @top.push_back $i + 1
        @bottom.push_back $i
      end
    end
    $max Int = @top[0]
    $min Int = @bottom[0]
    for $i in @top[1..@top.size]
      if cmd $max, $i
        $max = $i
      end
    end
    for $i in @bottom[1..@bottom.size]
      if cmd $i, $min
        $min = $i
      end
    end
    if $T%2 == 1
      if cmd $max, $T
        $max = $T
        os.println "! %d %d", $min, $max
        continue
      end
      if cmd $T, $min
        $min = $T
      end
    end
    os.println "! %d %d", $min, $max
  end
end
