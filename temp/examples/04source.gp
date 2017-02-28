do
	$sum: Int = 0;
	@a: Int[50];
	for $i in @a do
		$i = 10;
		$sum += $i;
	end #Странности какие-то

	if ($sum >= 500) do
		$sum -= 500;
	end
end
