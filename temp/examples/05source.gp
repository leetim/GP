type
  SomeRecord = record{
    $number: Int;
    @points: Int[10];
  }
var
  $sr: SomeRecord;

def getSomeRecord($number: Int, @points: Int[10])
  $result: SomeRecord;
  $result.$number = $number;
  $result.@points = @points;
  return $result;
end

do
  $sr = getSomeRecord(10, [15, 14]);
end
