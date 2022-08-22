#ifndef  CRAILS_HTML_TEMPLATE_HPP
# define CRAILS_HTML_TEMPLATE_HPP

# include <crails/template.hpp>
# include <map>
# include <functional>

namespace Crails
{
  class HtmlTemplate : public Template
  {
  public:
    typedef std::function<std::string()> Yieldable;

    HtmlTemplate(const Renderer* renderer, SharedVars& vars) :
      Template(renderer, vars)
    {
    }
    
    static std::string html_escape(const std::string& data);
    static std::string tag(const std::string& name, const std::map<std::string, std::string>& attrs);
    static std::string tag(const std::string& name, const std::map<std::string, std::string>& attrs, Yieldable);
    static std::string tag(const std::string& name, Yieldable);
    std::string        form(const std::map<std::string, std::string>& attrs, Yieldable) const;
  };
}

#endif