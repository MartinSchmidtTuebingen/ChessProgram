dir=$(pwd)

builddir=$(dirname $dir)/BUILDCHESSPROGRAM
if [ ! -d $builddir ];then
  mkdir $builddir
fi
cp $dir/*.cpp $builddir -u
cp $dir/*.h $builddir -u
cp $dir/Makefile $builddir -u

cd $builddir
make
cd $dir