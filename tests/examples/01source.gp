type
  Dirs = {
    diLeft,
    diRight,
    diTop,
    diBottom
  };
var
  $i: Int;

def func(@arg: Long[20])
  $t: Int = 0;
  for $i in @arg do
    $t += $i;
  end
  return $t;
end

do
  $t: Long = 0;
  for $i in [0..10] do
    $t += $i;
  end
  while $t > 20 do
    $t /= 2;
  end
  if $t < 15 do
    $t = 5 + $t;
  end
end
