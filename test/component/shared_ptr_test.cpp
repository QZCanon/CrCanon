
#include "shared_ptr.h"
#include <memory>

using namespace Canon::Ptr;
using namespace std;

class node
{
public:
    node() {}
    node(int val) : value(val) {}
    void print() { cout << value << endl; }

private:
    int value = 0;
};

int main()
{
    // SharedPtr<int> ptr = SharedPtr<int>(new int);

    // cout << ptr << endl;

    // shared_ptr<int> p = make_shared<int>(1);

    // cout << *p.get() << endl;

    auto sptr = MakeShared<int>(100);

    auto p2 = sptr;

    cout << p2.use_count() << endl;

    return 0;
}