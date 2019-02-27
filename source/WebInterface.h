#pragma once

#include <algorithm>
#include <experimental/optional>
#include <string>

#include "base/vector.h"
#include "web/Button.h"
#include "web/Input.h"
#include "web/web.h"
#include "web/Animate.h"
#include "tools/math.h"
#include "tools/Random.h"

#include "Config.h"
#include "DishWorld.h"
#include "WebArtist.h"

namespace UI = emp::web;

class WebInterface : public UI::Animate {

  Config cfg;

  UI::Document button_dash;  //< Div that contains the button dashboard.
  UI::Document systematics_dash;  //< Div that contains the systematics info.
  UI::Document dominant_viewer;

  DishWorld w;

  UI::Document channel_viewer;
  UI::Document wave_viewer;
  UI::Document stockpile_viewer;
  UI::Document contribution_viewer;
  WebArtist<ChannelPack> channel;
  emp::vector<WebArtist<int>> wave;
  WebArtist<double> stockpile;
  WebArtist<double> contribution;

  bool rendered;
  bool downloaded;

  std::string ChannelColor(ChannelPack &cp) {
    return emp::ColorHSV(
      emp::Mod(cp[1],360.0),
      emp::Mod(cp[0],0.6)+0.4,
      1.0
    );
  }

public:

  WebInterface()
    : button_dash("emp_button_dash")
    , systematics_dash("emp_systematics_dash")
    , dominant_viewer("dominant_viewer")
    , w(cfg)
    , channel_viewer("channel_viewer")
    , wave_viewer("wave_viewer")
    , stockpile_viewer("stockpile_viewer")
    , contribution_viewer("contribution_viewer")
    , channel(
      channel_viewer,
      [this](size_t i){
        return w.man->Channel(i).GetIDs();
      },
      [this](std::experimental::optional<ChannelPack> cp) {
        return cp ? ChannelColor(*cp) : "black";
      },
      cfg,
      [this](std::experimental::optional<ChannelPack> cp1, std::experimental::optional<ChannelPack> cp2) -> std::string {
        if (!cp1 || !cp2) return "black";
        else if ((*cp1)[0] == (*cp2)[0]) return ChannelColor(*cp1);
        else if ((*cp1)[1] == (*cp2)[1]) return "white";
        else return "black";
      }
    ), stockpile(
      stockpile_viewer,
      [this](size_t i){
        return w.IsOccupied(i) ? std::experimental::make_optional(w.man->Stockpile(i).QueryResource()) : std::experimental::nullopt;
      },
      [this](std::experimental::optional<double> amt) -> std::string {
        if (amt) {
          if (*amt > cfg.REP_THRESH()) return emp::ColorRGB(
              std::min(255.0,(*amt - cfg.REP_THRESH())*25),
              255,
              0
            );
          else if (*amt > 0) return emp::ColorHSV(
            240,
            (*amt/ cfg.REP_THRESH()),
            1.0
          );
          else if (*amt == 0) return "white";
          else return emp::ColorRGB(
              255,
              255-std::min(255.0,(*amt)*255.0/cfg.KILL_THRESH()),
              255-std::min(255.0,(*amt)*255.0/cfg.KILL_THRESH())
            );
        } else return emp::ColorRGB(0,0,0);
      },
      cfg
    ), contribution(
      contribution_viewer,
      [this](size_t i){
        return w.IsOccupied(i) ? std::experimental::make_optional(w.man->Stockpile(i).QueryTotalContribute()) : std::experimental::nullopt;
      },
      [this](std::experimental::optional<double> amt) -> std::string {
        if (amt) {
          if (*amt > cfg.REP_THRESH()) return "yellow";
          else if (*amt > 0) return emp::ColorHSV(
            240.0-180.0*(*amt)/cfg.REP_THRESH(),
            1.0,
            1.0
          );
          else if (*amt == 0) return "white";
          else return "red";
        } else return "black";
      },
      cfg
    ) {

      cfg.Set("SNAPSHOT_FREQUENCY", "1000");
      cfg.Set("SNAPSHOT_LENGTH", "1");

    for (size_t l = 0; l < cfg.NLEV(); ++l) {
      wave.emplace_back(
          wave_viewer,
          [this, l](size_t i){
            return w.IsOccupied(i) ? std::experimental::make_optional(w.man->Wave(i,l).GetState()) : std::experimental::nullopt;
          },
          [this, l](std::experimental::optional<int> cp) -> std::string {
            if (cp) {
              if (*cp > 0 && *cp < cfg.Lev(l).EVENT_RADIUS()) {
                return emp::ColorRGB(0,255,0);
              } else if (*cp > 0) {
                return emp::ColorRGB(255,0,0);
              } else if (*cp < 0) {
                return emp::ColorRGB(0,0,255);
              } else {
                return "white";
              }
            } else {
              return "black";
            }
          },
          cfg
      );
    }

    auto version_text = button_dash.AddText("version_text");
    version_text << STRINGIFY(GIT_VERSION_);

    auto seed_text = button_dash.AddText("seed_text");
    seed_text << emp::to_string(cfg.SEED());

    auto ud_text = button_dash.AddText("ud_text");
    ud_text << "Update: " << UI::Live([this](){ return w.GetUpdate(); });

    button_dash << "&nbsp;" << UI::Button(
      [this](){ DoFrame(); },
      "Step"
    );
    button_dash << "&nbsp;" << GetToggleButton("Animate");
    button_dash << "&nbsp;" << UI::Input(
      [this](std::string in){ ; },
      "checkbox",
      "Render?",
      "render_input"
    );
    button_dash << "&nbsp;" << UI::Input(
      [this](std::string _){
        if(button_dash.Input("download_input").GetCurrValue() == "true") {
          Download();
        }
      },
      "checkbox",
      "Download snapshots?",
      "download_input"
    );

    auto sys_text = systematics_dash.AddText("sys_text");
    sys_text << " Num Unique Orgs: " << UI::Live([this](){
      return w.GetSystematics("systematics")->GetNumActive();
    });
    sys_text << " Ave Depth: " << UI::Live([this](){
      return w.GetSystematics("systematics")->GetAveDepth();
    });
    sys_text << " Num Roots: " << UI::Live([this](){
      return w.GetSystematics("systematics")->GetNumRoots();
    });
    sys_text << " MRCA Depth: " << UI::Live([this](){
      return w.GetSystematics("systematics")->GetMRCADepth();
    });

    auto dom_text = dominant_viewer.AddText("dom_text");
    dom_text << UI::Live([this](){

      std::map<Genome, size_t> counts;

      for(size_t pos = 0; pos < w.GetSize(); ++pos) {
        if (w.IsOccupied(pos)) counts[w.GetOrg(pos)] ++;
      }

      using pair_type = decltype(counts)::value_type;
      auto pr = std::max_element(
        std::begin(counts),
        std::end(counts),
        [](const pair_type & p1, const pair_type & p2) {
          return p1.second < p2.second;
        }
      );

      std::ostringstream buffer;
      pr->first.program.PrintProgram(buffer);

      return "COUNT:" + emp::to_string(pr->second) + "\n\n" + buffer.str();
    });

  }

  void DoFrame() {
    rendered = false;
    downloaded = false;

    w.Update();

    button_dash.Text("ud_text").Redraw();

    if (button_dash.Input("render_input").GetCurrValue() == "true") {
      Redraw();
    }

    if (button_dash.Input("download_input").GetCurrValue() == "true") {
      Download();
    }
  }

  void Redraw() {

    if (rendered) return;

    systematics_dash.Text("sys_text").Redraw();
    dominant_viewer.Text("dom_text").Redraw();
    channel.Redraw();
    stockpile.Redraw();
    contribution.Redraw();
    for(auto &w : wave) w.Redraw();

    rendered = true;

  }


  void Download() {

    if (downloaded) return;

    const std::string prefix = (
      std::string()
      + STRINGIFY(GIT_VERSION_)
      + "+"
      + "seed"
      + emp::to_string(cfg.SEED())
      + "-"
      + "update"
      + emp::to_string(w.GetUpdate())
      + "-"
    );

    if(cfg.TimingFun(w.GetUpdate())) {
      if (!rendered) Redraw();
      channel.Download(prefix + "channel");
      stockpile.Download(prefix + "stockpile");
      contribution.Download(prefix + "contribution");
    }

    downloaded = true;

  }

};
