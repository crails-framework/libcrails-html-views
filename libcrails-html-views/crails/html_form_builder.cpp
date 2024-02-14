#include "html_form_builder.hpp"

using namespace Crails;
using namespace std;

string FormBuilderBase::scoped_name(const string& name) const
{
  size_t index = name.find('[');

  if (index == 0)
    return string(scope) + name;
  if (index != string::npos)
    return string(scope) + '[' + name.substr(0, index) + ']' + name.substr(index);
  return string(scope) + '[' + name + ']';
}
