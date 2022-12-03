#include "html_template.hpp"
#include <crails/params.hpp>
#include <sstream>
#include <regex>
#include <chrono>
#include <ctime> // std::localtime
#include <iomanip> // std::put_time

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

static std::string get_inline_method_for_form(const std::map<std::string, std::string>& attrs)
{
  static const regex put_method_matcher("put", regex_constants::icase);
  static const regex delete_method_matcher("delete", regex_constants::icase);
  string inline_method;

  if (attrs.find("method") != attrs.end())
  {
    if (regex_match(attrs.at("method"), put_method_matcher))
      inline_method = "PUT";
    else if (regex_match(attrs.at("method"), delete_method_matcher))
      inline_method = "DELETE";
  }
  return inline_method;
}

static std::string form_link(const HtmlTemplate& tpl, const std::string& url, const std::string& label, std::map<std::string,std::string> attrs)
{
  static const string submit_javascript = "this.parentNode.submit()";
  auto method = attrs.extract("method").mapped();
  auto confirm_message = attrs.find("confirm");

  if (confirm_message == attrs.end())
    attrs["onclick"] = "javascript:" + submit_javascript;
  else
  {
    attrs["data-confirm"] = confirm_message->second;
    attrs["onclick"] = "javascript:if(confirm(this.dataset.confirm)){" + submit_javascript + '}';
    attrs.erase(confirm_message);
  }
  return tpl.form({{"method",method}, {"action",url}}, [&label, &attrs]() -> std::string
  {
    return HtmlTemplate::tag("a", attrs, [&label]() { return label; });
  });
}

string Crails::HtmlTemplate::link(const std::string& url, std::string label, std::map<std::string, std::string> attrs) const
{
  string method = attrs.find("method") != attrs.end() ? attrs.at("method") : "GET";

  if (label.length() == 0)
    label = url;
  if (method != "GET" && method != "get")
    return form_link(*this, url, label, attrs);
  attrs["href"] = url;
  return tag("a", attrs, [&label]() { return label; });
}

string Crails::HtmlTemplate::link(const std::string& url, std::map<std::string, std::string> attrs, Yieldable content) const
{
  return link(url, content(), attrs);
}

string Crails::HtmlTemplate::form(const std::map<std::string, std::string>& attrs, Crails::HtmlTemplate::Yieldable content) const
{
  map<string, string> definitive_attrs = attrs;
  string inline_method = get_inline_method_for_form(attrs);

  if (inline_method.length() > 0)
    definitive_attrs["method"] = "post";
  return tag("form", definitive_attrs, [this, inline_method, content]()
  {
    stringstream html_stream;

    if (inline_method.length() > 0)
      html_stream << tag("input", {{"type","hidden"},{"name","_method"},{"value",inline_method}});
    if (vars.find("params") != vars.end())
    {
      Params& params = *(cast<Params*>(vars, "params"));
      string  csrf_token = params.get_session()["csrf-token"].defaults_to<string>("");

      html_stream << tag("input", {{"type","hidden"},{"name","csrf-token"},{"value",csrf_token}});
    }
    if (content)
      html_stream << content();
    return html_stream.str();
  });
}

string Crails::HtmlTemplate::text_field(const std::string& name, const std::string& value, std::map<std::string,std::string> attrs) const
{
  attrs.merge(map<string,string>{{"type","text"},{"name",name},{"value",value}});
  return tag("input", attrs);
}

string Crails::HtmlTemplate::text_area(const std::string& name, const std::string& value, std::map<std::string,std::string> attrs) const
{
  attrs.merge(map<string,string>{{"name",name}});
  return tag("textarea", attrs, [&value]() { return html_escape(value); });
}

string Crails::HtmlTemplate::password_field(const std::string& name, const std::string& value, std::map<std::string,std::string> attrs) const
{
  attrs.merge(map<string,string>{{"type","password"},{"name",name},{"value",value}});
  return tag("input", attrs);
}

string Crails::HtmlTemplate::date_field(const string& name, time_t value, std::map<std::string,std::string> attrs) const
{
  static const string format("%Y-%m-%d");
  stringstream value_stream;

  if (value != 0)
    value_stream << put_time(localtime(&value), format.c_str());
  attrs.merge(map<string,string>{{"type","date"},{"name",name},{"value",value_stream.str()}});
  return tag("input", attrs);
}
