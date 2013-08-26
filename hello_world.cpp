#include <boost/lambda/lambda.hpp>
#include <iostream>
#include <iterator>
#include <algorithm>

int main()
{
    using namespace boost::lambda;
    typedef std::istream_iterator<int> in;

    std::cout << "Sample for Lambda. Enter a number\n";
    std::for_each(
        in(std::cin), in(), std::cout << (_1 * 3) << " " );
}
