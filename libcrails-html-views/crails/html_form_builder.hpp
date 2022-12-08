#ifndef  CRAILS_HTML_FORM_BUILDER_HPP
# define CRAILS_HTML_FORM_BUILDER_HPP

# include "html_template.hpp"

namespace Crails
{
  template<class MODEL>
  class FormBuilder
  {
    HtmlTemplate& html_template;
    MODEL&        model;
  public:
    FormBuilder(HtmlTemplate* parent, MODEL& model) : html_template(*parent), model(model) {}
    std::string scoped_name(const std::string& name) const { return std::string(MODEL::scope) + '[' + name + ']'; }
    std::string label_for(const std::string& name) const { return label_for(name, [&name]() { return name; }); }
    std::string label_for(const std::string& name, HtmlTemplate::Yieldable yieldable) const { return html_template.tag("label", {{"for",scoped_name(name)}}, yieldable); }
    std::string text_field(const std::string& name, const std::string& value, std::map<std::string,std::string> attrs = {}) const { return html_template.text_field(scoped_name(name), value, attrs); }
    std::string text_area(const std::string& name, const std::string& value, std::map<std::string,std::string> attrs = {}) const { return html_template.text_area(scoped_name(name), value, attrs); }
    std::string date_field(const std::string& name, std::time_t value, std::map<std::string,std::string> attrs = {}) const { return html_template.date_field(scoped_name(name), value, attrs); }
    std::string boolean_field(const std::string& name, bool value, std::map<std::string,std::string> attrs = {}) const { return html_template.boolean_field(scoped_name(name), value, attrs); }
    std::string password_field(const std::string& name, const std::string& value, std::map<std::string,std::string> attrs = {}) const { return html_template.password_field(scoped_name(name), value, attrs); }
    template<typename METHOD> std::string text_field(const std::string& name, METHOD method, std::map<std::string,std::string> attrs = {}) const { return html_template.text_field(scoped_name(name), (model.*method)(), attrs); }
    template<typename METHOD> std::string text_area(const std::string& name, METHOD method, std::map<std::string,std::string> attrs = {}) const { return html_template.text_area(scoped_name(name), (model.*method)(), attrs); }
    template<typename METHOD> std::string date_field(const std::string& name, METHOD method, std::map<std::string,std::string> attrs = {}) const { return html_template.date_field(scoped_name(name), (model.*method)(), attrs); }
    template<typename METHOD> std::string boolean_field(const std::string& name, METHOD method, std::map<std::string,std::string> attrs = {}) const { return html_template.boolean_field(scoped_name(name), (model.*method)(), attrs); }
    template<typename METHOD> std::string number_field(const std::string& name, METHOD method, std::map<std::string,std::string> attrs = {}) const { return html_template.number_field(scoped_name(name), (model.*method)(), attrs); }
    template<typename METHOD> std::string password_field(const std::string& name, METHOD method, std::map<std::string,std::string> attrs = {}) const { return html_template.password_field(scoped_name(name), (model.*method)(), attrs); }
    template<typename VALUE, typename METHOD> std::string select_field(const std::string& name, const std::map<VALUE, std::string>& options, METHOD method, std::map<std::string,std::string> attrs = {}) const { return html_template.select_field<VALUE>(scoped_name(name), options, (model.*method)(), attrs); }
  };
}

#endif
