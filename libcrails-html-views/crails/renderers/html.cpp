#include "html.hpp"

using namespace std;
using namespace Crails;

bool HtmlRenderer::can_render(const std::string& accept_header, const std::string& view) const
{
  if (accept_header.find("text/html") != std::string::npos
   || accept_header.find("*/*")       != std::string::npos
   || accept_header.find("text/*")    != std::string::npos)
    return templates.find(view) != templates.end();
  return (false);
}

void HtmlRenderer::render_template(const std::string& view, RenderTarget& target, SharedVars& vars) const
{
  auto   tpl       = templates.find(view);
  string layout    = cast<std::string>(vars, "layout", "");

  target.set_header("Content-Type", "text/html");
  if (layout != "" && view != layout)
  {
    if (can_render("text/html", layout))
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
