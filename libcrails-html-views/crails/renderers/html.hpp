#ifndef  CRAILS_HTML_RENDERER_HPP
# define CRAILS_HTML_RENDERER_HPP

# include <crails/renderer.hpp>

namespace Crails
{
  class HtmlRenderer : public Renderer
  {
  public:
    std::string_view get_name() const override { return "crails/html"; }
    const std::vector<std::string>& get_mimetypes() const override;
    void render_template(const std::string& view, RenderTarget&, SharedVars& vars) const override;
  };
}

#endif
