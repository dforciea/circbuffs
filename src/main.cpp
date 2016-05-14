#include <CircBuf.h>

using namespace std;
using namespace circbuffs;

int main(int argc, char** argv)
{
   CircBuf c(1048576);
   CircBuf::Path path1("foo");
   CircBuf::Path path2("foo/bar");
   
   return 0;
}
