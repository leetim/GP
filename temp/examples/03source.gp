do
  @a: Double[20];
  $t: Float = 1.242E10;
  for $i in [1...10]
    $t /= $i + 10;
  end
  $t = $t+@[1].to_float();
end
