#ifndef  CRAILS_HTML_RENDERER_HPP
# define CRAILS_HTML_RENDERER_HPP

# include <crails/renderer.hpp>

namespace Crails
{
  class HtmlRenderer : public Renderer
  {
  public:
    const std::vector<std::string>& get_mimetypes() const override;
    void render_template(const std::string& view, RenderTarget&, SharedVars& vars) const override;
  };
}

#endif
