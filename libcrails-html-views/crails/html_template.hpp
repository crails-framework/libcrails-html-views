#ifndef  CRAILS_HTML_TEMPLATE_HPP
# define CRAILS_HTML_TEMPLATE_HPP

# include <crails/template.hpp>
# include <map>
# include <functional>
# include <boost/lexical_cast.hpp>

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
    std::string        link(const std::string& url, std::string label = "", std::map<std::string, std::string> attrs = {}) const;
    std::string        link(const std::string& url, std::map<std::string, std::string> attrs, Yieldable) const;
    std::string        link(const std::string& url, Yieldable content) const { return link(url, {}, content); }

    template<typename MODEL>
    std::string form_for(const MODEL& model, const std::string& route, Yieldable yieldable) const
    {
      return form_for(model, route, {}, yieldable);
    }

    template<typename MODEL>
    std::string form_for(const MODEL& model, const std::string& route, std::map<std::string, std::string> attrs, Yieldable yieldable) const
    {
      auto method_and_route = model.is_persistent()
        ? std::pair<std::string, std::string>{"put",  route + '/' + boost::lexical_cast<std::string>(model.get_id())}
        : std::pair<std::string, std::string>{"post", route};

      if (attrs.find("method") == attrs.end())
        attrs["method"] = method_and_route.first;
      if (attrs.find("action") == attrs.end())
        attrs["action"] = method_and_route.second;
      return form(attrs, yieldable);
    }

    std::string text_field(const std::string& name, const std::string& value, std::map<std::string, std::string> attrs = {}) const;
    std::string text_area(const std::string& name, const std::string& value, std::map<std::string, std::string> attrs = {}) const;
    std::string date_field(const std::string& name, std::time_t value, std::map<std::string, std::string> attrs = {}) const;
    std::string password_field(const std::string& name, const std::string& value, std::map<std::string, std::string> attrs = {}) const;
    template<typename VALUE> std::string number_field(const std::string& name, VALUE value, std::map<std::string, std::string> attrs = {}) { attrs.merge(std::map<std::string,std::string>{{"type","number"},{"name",name},{"value",boost::lexical_cast<std::string>(value)}}); return tag("input", attrs); }

    template<typename VALUE> std::string select_field(const std::string& name, const std::map<VALUE, std::string>& options, VALUE selected_value, std::map<std::string, std::string> attrs = {})
    {
      attrs["name"] = name;
      return tag("select", attrs, [&]() -> std::string
      {
        std::string result;
        for (auto it = options.begin() ; it != options.end() ; ++it)
        {
          std::map<std::string, std::string> option_attrs{
            {"value", boost::lexical_cast<std::string>(it->first)}
          };

          if (it->first == selected_value)
            option_attrs["selected"] = "selected";
          result += tag("option", option_attrs, [it]() { return it->second; });
        }
        return result;
      });
    }
  };
}

#endif
