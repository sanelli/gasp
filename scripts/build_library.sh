for f in ../build/gasp/library/*.mblaise
do
  tgt=${f%.mblaise}.tb
  echo "Compiling '$f' => '$tgt'"
  ../build/gasp/gasp compile -i $f -of torricelly-binary > $tgt
done
