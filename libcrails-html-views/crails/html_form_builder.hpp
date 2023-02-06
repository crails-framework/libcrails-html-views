#ifndef  CRAILS_HTML_FORM_BUILDER_HPP
# define CRAILS_HTML_FORM_BUILDER_HPP

# include "html_template.hpp"

namespace Crails
{
  class FormBuilderBase
  {
  protected:
    HtmlTemplate&      html_template;
    const std::string& scope;
  public:
    FormBuilderBase(HtmlTemplate* parent, const std::string& scope) : html_template(*parent), scope(scope) {}
    std::string scoped_name(const std::string& name) const { return scope + '[' + name + ']'; }
    std::string label_for(const std::string& name, const std::string& label) const { return label_for(name, [&label]() { return label; }); }
    std::string label_for(const std::string& name) const { return label_for(name, name); }
    std::string label_for(const std::string& name, HtmlTemplate::Yieldable yieldable) const { return html_template.tag("label", {{"for",scoped_name(name)}}, yieldable); }
    std::string hidden_field(const std::string& name, const std::string& value, std::map<std::string, std::string> attrs = {}) const { return html_template.hidden_field(scoped_name(name), value, attrs); }
    std::string text_field(const std::string& name, const std::string& value, std::map<std::string,std::string> attrs = {}) const { return html_template.text_field(scoped_name(name), value, attrs); }
    std::string text_area(const std::string& name, const std::string& value, std::map<std::string,std::string> attrs = {}) const { return html_template.text_area(scoped_name(name), value, attrs); }
    std::string mail_field(const std::string& name, const std::string& value, std::map<std::string,std::string> attrs = {}) const { return html_template.mail_field(scoped_name(name), value, attrs); }
    std::string date_field(const std::string& name, std::time_t value, std::map<std::string,std::string> attrs = {}) const { return html_template.date_field(scoped_name(name), value, attrs); }
    std::string datetime_field(const std::string& name, const std::string& value, std::map<std::string, std::string> attrs = {}) const { return html_template.datetime_field(scoped_name(name), value, attrs); }
    std::string boolean_field(const std::string& name, bool value, std::map<std::string,std::string> attrs = {}) const { return html_template.boolean_field(scoped_name(name), value, attrs); }
    std::string password_field(const std::string& name, const std::string& value, std::map<std::string,std::string> attrs = {}) const { return html_template.password_field(scoped_name(name), value, attrs); }
    std::string upload_field(const std::string& name, std::map<std::string,std::string> attrs = {}) const { return html_template.upload_field(scoped_name(name)); }
    std::string multiple_upload_field(const std::string& name, std::map<std::string,std::string> attrs = {}) const { return html_template.multiple_upload_field(scoped_name(name)); }
    template<typename VALUE> std::string select_field(const std::string& name, const std::map<VALUE, std::string>& options, const std::vector<VALUE>& values, std::map<std::string,std::string> attrs = {}) const { return html_template.select_field<VALUE>(scoped_name(name), options, values, attrs); }
    template<typename VALUE> std::string select_field(const std::string& name, const std::map<VALUE, std::string>& options, VALUE value, std::map<std::string,std::string> attrs = {}) const { return html_template.select_field<VALUE>(scoped_name(name), options, value, attrs); }
  };

  template<class MODEL>
  class FormBuilder : public FormBuilderBase
  {
  protected:
    const MODEL& model;
  public:
    FormBuilder(HtmlTemplate* parent, const MODEL& model) : FormBuilderBase(parent, MODEL::scope), model(model) {}
    template<typename METHOD> std::string hidden_field(const std::string& name, METHOD method, std::map<std::string,std::string> attrs = {}) const { return html_template.hidden_field(scoped_name(name), (model.*method())(), attrs); }
    template<typename METHOD> std::string text_field(const std::string& name, METHOD method, std::map<std::string,std::string> attrs = {}) const { return html_template.text_field(scoped_name(name), (model.*method)(), attrs); }
    template<typename METHOD> std::string text_area(const std::string& name, METHOD method, std::map<std::string,std::string> attrs = {}) const { return html_template.text_area(scoped_name(name), (model.*method)(), attrs); }
    template<typename METHOD> std::string mail_field(const std::string& name, METHOD method, std::map<std::string,std::string> attrs = {}) const { return html_template.mail_field(scoped_name(name), (model.*method)(), attrs); }
    template<typename METHOD> std::string date_field(const std::string& name, METHOD method, std::map<std::string,std::string> attrs = {}) const { return html_template.date_field(scoped_name(name), (model.*method)(), attrs); }
    template<typename METHOD> std::string datetime_field(const std::string& name, METHOD method, std::map<std::string,std::string> attrs = {}) const { return html_template.datetime_field(scoped_name(name), (model.*method)(), attrs); }
    template<typename METHOD> std::string boolean_field(const std::string& name, METHOD method, std::map<std::string,std::string> attrs = {}) const { return html_template.boolean_field(scoped_name(name), (model.*method)(), attrs); }
    template<typename METHOD> std::string number_field(const std::string& name, METHOD method, std::map<std::string,std::string> attrs = {}) const { return html_template.number_field(scoped_name(name), (model.*method)(), attrs); }
    template<typename METHOD> std::string password_field(const std::string& name, METHOD method, std::map<std::string,std::string> attrs = {}) const { return html_template.password_field(scoped_name(name), (model.*method)(), attrs); }
    template<typename VALUE, typename METHOD> std::string select_field(const std::string& name, const std::map<VALUE, std::string>& options, METHOD method, std::map<std::string,std::string> attrs = {}) const { return html_template.select_field<VALUE>(scoped_name(name), options, (model.*method)(), attrs); }

    // Following is a copy-paste from FormBuilderBase methods, required due to the overload shadowing
    std::string label_for(const std::string& name, const std::string& label) const { return label_for(name, [&label]() { return label; }); }
    std::string label_for(const std::string& name) const { return label_for(name, name); }
    std::string label_for(const std::string& name, HtmlTemplate::Yieldable yieldable) const { return html_template.tag("label", {{"for",scoped_name(name)}}, yieldable); }
    std::string hidden_field(const std::string& name, const std::string& value, std::map<std::string, std::string> attrs = {}) const { return html_template.hidden_field(scoped_name(name), value, attrs); }
    std::string text_field(const std::string& name, const std::string& value, std::map<std::string,std::string> attrs = {}) const { return html_template.text_field(scoped_name(name), value, attrs); }
    std::string text_area(const std::string& name, const std::string& value, std::map<std::string,std::string> attrs = {}) const { return html_template.text_area(scoped_name(name), value, attrs); }
    std::string mail_field(const std::string& name, const std::string& value, std::map<std::string,std::string> attrs = {}) const { return html_template.mail_field(scoped_name(name), value, attrs); }
    std::string date_field(const std::string& name, std::time_t value, std::map<std::string,std::string> attrs = {}) const { return html_template.date_field(scoped_name(name), value, attrs); }
    std::string datetime_field(const std::string& name, const std::string& value, std::map<std::string, std::string> attrs = {}) const { return html_template.datetime_field(scoped_name(name), value, attrs); }
    std::string boolean_field(const std::string& name, bool value, std::map<std::string,std::string> attrs = {}) const { return html_template.boolean_field(scoped_name(name), value, attrs); }
    std::string password_field(const std::string& name, const std::string& value, std::map<std::string,std::string> attrs = {}) const { return html_template.password_field(scoped_name(name), value, attrs); }
    std::string upload_field(const std::string& name, std::map<std::string,std::string> attrs = {}) const { return html_template.upload_field(scoped_name(name)); }
    std::string multiple_upload_field(const std::string& name, std::map<std::string,std::string> attrs = {}) const { return html_template.multiple_upload_field(scoped_name(name)); }
    template<typename VALUE> std::string select_field(const std::string& name, const std::map<VALUE, std::string>& options, const std::vector<VALUE>& values, std::map<std::string,std::string> attrs = {}) const { return html_template.select_field<VALUE>(scoped_name(name), options, values, attrs); }
    template<typename VALUE> std::string select_field(const std::string& name, const std::map<VALUE, std::string>& options, VALUE value, std::map<std::string,std::string> attrs = {}) const { return html_template.select_field<VALUE>(scoped_name(name), options, value, attrs); }
  };
}

#endif
