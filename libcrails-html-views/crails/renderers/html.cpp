#include "html.hpp"
#include <iostream>

using namespace std;
using namespace Crails;

const std::vector<std::string>& HtmlRenderer::get_mimetypes() const
{
  static const std::vector<std::string> mimetypes{
    "text/html", "application/xhtml+xml"
  };
  return mimetypes;
}

void HtmlRenderer::render_template(const std::string& view, RenderTarget& target, SharedVars& vars) const
{
  auto   tpl       = templates.find(view);
  string layout    = cast<std::string>(vars, "layout", "");

  target.set_header("Content-Type", "text/html");
  if (layout != "" && view != layout)
  {
    if (can_render(layout))
    {
      RenderString yield;

      tpl->second(*this, yield, vars);
      vars["yield"] = yield.c_str();
      render_template(layout, target, vars);
    }
    else
      throw MissingTemplate(layout);
  }
  else
    tpl->second(*this, target, vars);
}
