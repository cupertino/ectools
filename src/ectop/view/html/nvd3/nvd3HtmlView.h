///////////////////////////////////////////////////////////////////////////////
/// @file               JsonGridView.h
/// @author             Dorian RODDE
/// @version    0.1
/// @date               2012.07
/// @copyright  2012, CoolEmAll (INFSO-ICT-288701)
/// @brief              Json GridModel view
///////////////////////////////////////////////////////////////////////////////

#ifndef nvd3HtmlView_H__
#define nvd3HtmlView_H__

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#include <libec/grid/view/GridView.h>

namespace cea
{
  class nvd3HtmlView : public GridView
  {
  public:

    nvd3HtmlView();
    nvd3HtmlView(GridModel& model);

    virtual void
    renderColumnHeader(const ColumnView& col, int colId,
        const std::string& text, int x, int y, int colSize,
        const GridStyle& style);

    virtual void
    renderRow(const RowView& row, int rowId, int x, int y, int viewWidth);
    virtual void
    renderCell(const GridCell& c, int rowId, int colId, const std::string& text,
        int x, int y, int colSize, const GridStyle& style);

    virtual void
    renderSumBar(int x, int y, int viewWidth);
    virtual void
    renderSumBarItem(const ColumnView& col, int colId, const std::string& text,
        int x, int y, int colSize, const GridStyle& style);

    virtual void
    beforeRender();

    virtual void
    afterRender(int viewWidth, int viewHeight);

    void
    setJsonSource(const std::string& header, const std::string& end);

    void
    setOutput(const std::string& outputFile);

    std::string
    getOutput() const;

    std::string
    getContent() const;

    bool
    saveToFile(const std::string& outputFile);

    virtual void
    loadSpecificXML(GridModel& model, XMLReader& xml,
        const XMLReader::Balise& balise);

    void createStaticFiles(std::string path);

  protected:
    virtual void
    getShowText(const std::string& txt, unsigned int colSize,
        const GridStyle& style, std::string& retText);

    std::string _Json; ///< Json result source
    std::string _JsonHeader; ///< Json Header source
    std::string _JsonEnd; ///< Json Footer source
    std::string _outputFile; ///< Json output File


  private:
    void createHtml(std::string path);
    void createCss(std::string path);
    void createJS(std::string path);
    void createJson(std::string path);

    GridModel* _model;
  };

}

#endif
