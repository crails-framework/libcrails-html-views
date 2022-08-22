#include "html_template.hpp"
#include <crails/params.hpp>
#include <sstream>

using namespace Crails;
using namespace std;

string HtmlTemplate::html_escape(const string& data)
{
  string buffer;

  buffer.reserve(data.size());
  for(size_t pos = 0; pos != data.size(); ++pos) {
    switch(data[pos]) {
      case '&':  buffer.append("&amp;");       break;
      case '\"': buffer.append("&quot;");      break;
      case '\'': buffer.append("&apos;");      break;
      case '<':  buffer.append("&lt;");        break;
      case '>':  buffer.append("&gt;");        break;
      default:   buffer.append(&data[pos], 1); break;
    }
  }
  return buffer;
}

string HtmlTemplate::tag(const string& name, const map<string, string>& attrs)
{
  return tag(name, attrs, Yieldable());
}

string HtmlTemplate::tag(const string& name, Yieldable content)
{
  return tag(name, {}, content);
}

string HtmlTemplate::tag(const string& name, const map<string, string>& attrs, Yieldable content)
{
  stringstream html_stream;

  html_stream << '<' << name;
  for (const auto& attr : attrs)
  {
    html_stream << ' ' << attr.first << "=\"";
    for (size_t i = 0 ; i < attr.second.length() ; ++i)
    {
      if (attr.second[i] == '\\' || attr.second[i] == '"')
        html_stream << '\\';
      html_stream << attr.second[i];
    }
    html_stream << '"';
  }
  html_stream << '>';
  if (content)
    html_stream << content();
  html_stream << "</" << name << '>';
  return html_stream.str();
}

string Crails::HtmlTemplate::form(const std::map<std::string, std::string>& attrs, Crails::HtmlTemplate::Yieldable content) const
{
  return tag("form", attrs, [this, content]()
  {
    stringstream html_stream;

    if (vars.find("params") != vars.end())
    {
      Params& params = *(cast<Params*>(vars, "params"));
      string  csrf_token = params.get_session()["csrf_token"].defaults_to<string>("");

      html_stream << tag("input", {{"type","hidden"},{"name","csrf-token"},{"value",csrf_token}});
    }
    if (content)
      html_stream << content();
    return html_stream.str();
  });
}
