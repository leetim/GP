package main

import os

def swap $a ref Int, $b ref Int
  $c Int
  $c = $a
  $a = $b
  $b = $c
end

do
  @a Int[] = [5, 4, 9, 2, 3]
  for $i in 0..@a.size
    for $j in $i+1..@a.size
      if @a[$i] < @a[$j]
        swap @a[$i], @a[$j]
      end
    end
  end
end
