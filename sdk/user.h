/**************************************************************************/
/*                                                                        */
/*                              WWIV Version 5.x                          */
/*             Copyright (C)1998-2021, WWIV Software Services            */
/*                                                                        */
/*    Licensed  under the  Apache License, Version  2.0 (the "License");  */
/*    you may not use this  file  except in compliance with the License.  */
/*    You may obtain a copy of the License at                             */
/*                                                                        */
/*                http://www.apache.org/licenses/LICENSE-2.0              */
/*                                                                        */
/*    Unless  required  by  applicable  law  or agreed to  in  writing,   */
/*    software  distributed  under  the  License  is  distributed on an   */
/*    "AS IS"  BASIS, WITHOUT  WARRANTIES  OR  CONDITIONS OF ANY  KIND,   */
/*    either  express  or implied.  See  the  License for  the specific   */
/*    language governing permissions and limitations under the License.   */
/*                                                                        */
/**************************************************************************/

#ifndef INCLUDED_SDK_USER_H
#define INCLUDED_SDK_USER_H

#include "core/datetime.h"
#include "core/ip_address.h"
#include "core/strings.h"
#include "sdk/vardec.h"
#include "sdk/net/net.h"

#include <chrono>
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>

//
// ListPlus options from lp_options.
//
static constexpr uint32_t cfl_fname = 0x00000001;
static constexpr uint32_t cfl_extension = 0x00000002;
static constexpr uint32_t cfl_dloads = 0x00000004;
static constexpr uint32_t cfl_kbytes = 0x00000008;
static constexpr uint32_t cfl_date_uploaded = 0x00000010;
static constexpr uint32_t cfl_file_points = 0x00000020;
static constexpr uint32_t cfl_days_old = 0x00000040;
static constexpr uint32_t cfl_upby = 0x00000080;
static constexpr uint32_t unused_cfl_times_a_day_dloaded = 0x00000100;
static constexpr uint32_t unused_cfl_days_between_dloads = 0x00000200;
static constexpr uint32_t cfl_description = 0x00000400;
static constexpr uint32_t cfl_header = 0x80000000;

constexpr int HOTKEYS_ON = 0;
constexpr int HOTKEYS_OFF = 1;

namespace wwiv::sdk {

/**
 * User Class - Represents a User record
 */
class User final {
 public:
  // Constants

  // USERREC.inact
  static constexpr int userDeleted                = 0x01;
  static constexpr int userInactive               = 0x02;

  // USERREC.exempt
  static constexpr int exemptRatio                = 0x01;
  static constexpr int exemptTime                 = 0x02;
  static constexpr int exemptPost                 = 0x04;
  static constexpr int exemptAll                  = 0x08;
  static constexpr int exemptAutoDelete           = 0x10;

  // USERREC.restrict
  static constexpr int restrictLogon              = 0x0001;
  static constexpr int restrictChat               = 0x0002;
  static constexpr int restrictValidate           = 0x0004;
  static constexpr int restrictAutomessage        = 0x0008;
  static constexpr int restrictAnony              = 0x0010;
  static constexpr int restrictPost               = 0x0020;
  static constexpr int restrictEmail              = 0x0040;
  static constexpr int restrictVote               = 0x0080;
  static constexpr int restrictMultiNodeChat      = 0x0100;
  static constexpr int restrictNet                = 0x0200;
  static constexpr int restrictUpload             = 0x0400;

  // USERREC.sysstatus
  static constexpr int ansi                       = 0x00000001;
  static constexpr int status_color               = 0x00000002;
  static constexpr int music                      = 0x00000004;
  static constexpr int pauseOnPage                = 0x00000008;
  static constexpr int expert                     = 0x00000010;
  static constexpr int SMW                        = 0x00000020;
  static constexpr int fullScreen                 = 0x00000040;
  static constexpr int nscanFileSystem            = 0x00000080;
  static constexpr int extraColor                 = 0x00000100;
  static constexpr int clearScreen                = 0x00000200;
  static constexpr int msg_show_controlcodes      = 0x00000400;
  static constexpr int unused_noTag               = 0x00000800;
  static constexpr int conference                 = 0x00001000;
  static constexpr int noChat                     = 0x00002000;
  static constexpr int noMsgs                     = 0x00004000;
  static constexpr int fullScreenReader           = 0x00008000;
  static constexpr int unused_listPlus            = 0x00010000;
  static constexpr int autoQuote                  = 0x00020000;
  static constexpr int twentyFourHourClock        = 0x00040000;
  static constexpr int msgPriority                = 0x00080000;  // not used?

  //
  // Data
  //
  struct userrec data{};

  //
  // Constructor and Destructor
  //
  User();
  ~User() = default;

  User(const User& w);
  explicit User(const userrec& rhs);
  User& operator=(const User& rhs);

  //
  // Member Functions
  //
  void FixUp();    // was function fix_user_rec
  void ZeroUserData();

  //
  // Accessor Functions
  //

  // USERREC.inact
  void SetInactFlag(int nFlag) {
    data.inact |= nFlag;
  }
  void ToggleInactFlag(int nFlag) {
    data.inact ^= nFlag;
  }
  void ClearInactFlag(int nFlag) {
    data.inact &= ~nFlag;
  }
  [[nodiscard]] bool IsUserDeleted() const {
    return (data.inact & User::userDeleted) != 0;
  }
  [[nodiscard]] bool IsUserInactive() const {
    return (data.inact & User::userInactive) != 0;
  }

  // USERREC.sysstatus
  void SetStatusFlag(int nFlag, bool on) {
    if (on) {
      data.sysstatus |= nFlag;
    } else {
      data.sysstatus &= ~nFlag;
    }
  }
  void SetStatusFlag(int nFlag) {
    data.sysstatus |= nFlag;
  }
  void ToggleStatusFlag(int nFlag) {
    data.sysstatus ^= nFlag;
  }
  void ClearStatusFlag(int nFlag) {
    data.sysstatus &= ~nFlag;
  }
  [[nodiscard]] bool HasStatusFlag(int nFlag) const {
    return (data.sysstatus & nFlag) != 0;
  }
  [[nodiscard]] long get_status() const {
    return static_cast<long>(data.sysstatus);
  }
  void SetStatus(long l) {
    data.sysstatus = static_cast<uint32_t>(l);
  }
  [[nodiscard]] bool HasAnsi() const {
    return HasStatusFlag(ansi);
  }
  [[nodiscard]] bool HasColor() const { return HasStatusFlag(status_color); }
  [[nodiscard]] bool HasMusic() const {
    return HasStatusFlag(music);
  }
  [[nodiscard]] bool HasPause() const {
    return HasStatusFlag(pauseOnPage);
  }
  [[nodiscard]] bool IsExpert() const {
    return HasStatusFlag(expert);
  }
  [[nodiscard]] bool HasShortMessage() const {
    return HasStatusFlag(SMW);
  }
  [[nodiscard]] bool IsFullScreen() const {
    return HasStatusFlag(fullScreen);
  }
  [[nodiscard]] bool IsNewScanFiles() const {
    return HasStatusFlag(nscanFileSystem);
  }
  [[nodiscard]] bool IsUseExtraColor() const {
    return HasStatusFlag(extraColor);
  }
  [[nodiscard]] bool IsUseClearScreen() const {
    return HasStatusFlag(clearScreen);
  }
  [[nodiscard]] bool IsUseConference() const {
    return HasStatusFlag(conference);
  }
  [[nodiscard]] bool IsIgnoreChatRequests() const {
    return HasStatusFlag(noChat);
  }
  [[nodiscard]] bool IsIgnoreNodeMessages() const {
    return HasStatusFlag(noMsgs);
  }

  [[nodiscard]] bool IsUseAutoQuote() const {
    return HasStatusFlag(autoQuote);
  }
  [[nodiscard]] bool IsUse24HourClock() const {
    return HasStatusFlag(twentyFourHourClock);
  }

  // USERREC.exempt
  void SetExemptFlag(int nFlag) {
    data.exempt |= nFlag;
  }
  void ToggleExemptFlag(int nFlag) {
    data.exempt ^= nFlag;
  }
  void ClearExemptFlag(int nFlag) {
    data.exempt &= ~nFlag;
  }
  [[nodiscard]] bool HasExemptFlag(int nFlag) const {
    return (data.exempt & nFlag) != 0;
  }

  [[nodiscard]] bool IsExemptRatio() const {
    return HasExemptFlag(exemptRatio);
  }
  [[nodiscard]] bool IsExemptTime() const {
    return HasExemptFlag(exemptTime);
  }
  [[nodiscard]] bool IsExemptPost() const {
    return HasExemptFlag(exemptPost);
  }
  [[nodiscard]] bool IsExemptAll() const {
    return HasExemptFlag(exemptAll);
  }
  [[nodiscard]] bool IsExemptAutoDelete() const {
    return HasExemptFlag(exemptAutoDelete);
  }

  // USERREC.restrict
  void SetRestrictionFlag(int nFlag) noexcept {
    data.restrict |= nFlag;
  }
  void ToggleRestrictionFlag(int nFlag) noexcept {
    data.restrict ^= nFlag;
  }
  void ClearRestrictionFlag(int nFlag) noexcept {
    data.restrict &= ~nFlag;
  }
  [[nodiscard]] bool HasRestrictionFlag(int nFlag) const noexcept {
    return (data.restrict & nFlag) != 0;
  }
  [[nodiscard]] uint16_t restriction() const {
    return data.restrict;
  }
  void restriction(uint16_t n) {
    data.restrict = n;
  }

  [[nodiscard]] bool IsRestrictionLogon() const {
    return HasRestrictionFlag(restrictLogon);
  }
  [[nodiscard]] bool IsRestrictionChat() const {
    return HasRestrictionFlag(restrictChat);
  }
  [[nodiscard]] bool IsRestrictionValidate() const {
    return HasRestrictionFlag(restrictValidate);
  }
  [[nodiscard]] bool IsRestrictionAutomessage() const {
    return HasRestrictionFlag(restrictAutomessage);
  }
  [[nodiscard]] bool IsRestrictionAnonymous() const {
    return HasRestrictionFlag(restrictAnony);
  }
  [[nodiscard]] bool IsRestrictionPost() const {
    return HasRestrictionFlag(restrictPost);
  }
  [[nodiscard]] bool IsRestrictionEmail() const {
    return HasRestrictionFlag(restrictEmail);
  }
  [[nodiscard]] bool IsRestrictionVote() const {
    return HasRestrictionFlag(restrictVote);
  }
  [[nodiscard]] bool IsRestrictionMultiNodeChat() const {
    return HasRestrictionFlag(restrictMultiNodeChat);
  }
  [[nodiscard]] bool IsRestrictionNet() const {
    return HasRestrictionFlag(restrictNet);
  }
  [[nodiscard]] bool IsRestrictionUpload() const {
    return HasRestrictionFlag(restrictUpload);
  }

  void ToggleArFlag(int nFlag) {
    data.ar ^= nFlag;
  }
  [[nodiscard]] bool HasArFlag(int ar) const {
    if (ar == 0) {
      // Always have the empty ar
      return true;
    }
    return (data.ar & ar) != 0;
  }
  [[nodiscard]] int ar_int() const {
    return data.ar;
  }
  void ar_int(int n) {
    data.ar = static_cast<uint16_t>(n);
  }

  void ToggleDarFlag(int nFlag) {
    data.dar ^= nFlag;
  }
  [[nodiscard]] bool HasDarFlag(int nFlag) const {
    if (nFlag == 0) {
      // Always have the empty dar
      return true;
    }
    return (data.dar & nFlag) != 0;
  }
  [[nodiscard]] int dar_int() const {
    return data.dar;
  }
  void dar_int(int n) {
    data.dar = static_cast<uint16_t>(n);
  }

  [[nodiscard]] const char *GetName() const {
    return reinterpret_cast<const char*>(data.name);
  }
  [[nodiscard]] std::string name() const {
    return std::string(reinterpret_cast<const char*>(data.name));
  }
  void set_name(const std::string& s) {
    strcpy(reinterpret_cast<char*>(data.name), s.c_str());
  }
  [[nodiscard]] std::string real_name() const {
    return std::string(reinterpret_cast<const char*>(data.realname));
  }
  void real_name(const std::string& s) {
    strcpy(reinterpret_cast<char*>(data.realname), s.c_str());
  }
  [[nodiscard]] std::string callsign() const {
    return std::string(reinterpret_cast<const char*>(data.callsign));
  }
  void callsign(const std::string& s) {
    strcpy(reinterpret_cast<char*>(data.callsign), s.c_str());
  }
  [[nodiscard]] const char *GetVoicePhoneNumber() const {
    return reinterpret_cast<const char*>(data.phone);
  }
  void SetVoicePhoneNumber(const char *s) {
    strcpy(reinterpret_cast<char*>(data.phone), s);
  }
  [[nodiscard]] const char *GetDataPhoneNumber() const {
    return reinterpret_cast<const char*>(data.dataphone);
  }
  void SetDataPhoneNumber(const char *s) {
    strcpy(reinterpret_cast<char*>(data.dataphone), s);
  }
  [[nodiscard]] const char *GetStreet() const {
    return reinterpret_cast<const char*>(data.street);
  }
  void SetStreet(const char *s) {
    strcpy(reinterpret_cast<char*>(data.street), s);
  }
  [[nodiscard]] const char *GetCity() const {
    return reinterpret_cast<const char*>(data.city);
  }
  void SetCity(const char *s) {
    strcpy(reinterpret_cast<char*>(data.city), s);
  }
  [[nodiscard]] const char *GetState() const {
    return reinterpret_cast<const char*>(data.state);
  }
  void SetState(const char *s) {
    strcpy(reinterpret_cast<char*>(data.state), s);
  }
  [[nodiscard]] const char *GetCountry() const {
    return reinterpret_cast<const char*>(data.country);
  }
  void SetCountry(const char *s) {
    strcpy(reinterpret_cast<char*>(data.country) , s);
  }
  [[nodiscard]] const char *GetZipcode() const {
    return reinterpret_cast<const char*>(data.zipcode);
  }
  void SetZipcode(const char *s) {
    strcpy(reinterpret_cast<char*>(data.zipcode), s);
  }
  [[nodiscard]] std::string password() const {
    return std::string(reinterpret_cast<const char*>(data.pw));
  }
  void password(const std::string& s) {
    strings::to_char_array(data.pw, s);
  }

  [[nodiscard]] std::string laston() const {
    return data.laston;
  }

  void laston(const std::string& s) {
    strings::to_char_array(data.laston, s);
  }

  [[nodiscard]] std::string firston() const {
    return data.firston;
  }
  
  void firston(const std::string& s) {
    strings::to_char_array(data.firston, s);
  }

  [[nodiscard]] std::string note() const {
    return reinterpret_cast<const char*>(data.note);
  }
  void note(const std::string& s) {
    strcpy(reinterpret_cast<char*>(data.note), s.c_str());
  }
  [[nodiscard]] std::string macro(int line) const {
    return std::string(reinterpret_cast<const char*>(data.macros[line]));
  }
  void macro(int nLine, const char *s) {
    memset(&data.macros[ nLine ][0], 0, 80);
    strcpy(reinterpret_cast<char*>(data.macros[ nLine ]), s);
  }
  [[nodiscard]] char GetGender() const {
    if (data.sex == 'N') {
      // N means unknown.  NEWUSER sets it to N to prompt the
      // user again.
      return 'N';
    }
    return data.sex == 'F' ? 'F' : 'M';
  }
  void SetGender(const char c) {
    data.sex = static_cast<uint8_t>(c);
  }
  [[nodiscard]] std::string email_address() const {
    return data.email;
  }
  void email_address(const std::string& s) {
    strcpy(data.email, s.c_str());
  }
  [[nodiscard]] uint8_t age() const;

  [[nodiscard]] int8_t GetComputerType() const {
    return data.comp_type;
  }
  void SetComputerType(int n) {
    data.comp_type = static_cast<int8_t>(n);
  }

  [[nodiscard]] int GetDefaultProtocol() const {
    return data.defprot;
  }
  void SetDefaultProtocol(int n) {
    data.defprot = static_cast<uint8_t>(n);
  }
  [[nodiscard]] int GetDefaultEditor() const {
    return data.defed;
  }
  void SetDefaultEditor(int n) {
    data.defed = static_cast<uint8_t>(n);
  }
  [[nodiscard]] int GetScreenChars() const {
    return data.screenchars == 0 ? 80 : data.screenchars;
  }
  void SetScreenChars(int n) {
    data.screenchars = static_cast<uint8_t>(n);
  }
  [[nodiscard]] int GetScreenLines() const {
    return data.screenlines == 0 ? 25 : data.screenlines;
  }
  void SetScreenLines(int n) {
    data.screenlines = static_cast<uint8_t>(n);
  }
  [[nodiscard]] int GetNumExtended() const {
    return data.num_extended;
  }
  void SetNumExtended(int n) {
    data.num_extended = static_cast<uint8_t>(n);
  }
  [[nodiscard]] int GetOptionalVal() const {
    return data.optional_val;
  }
  void SetOptionalVal(int n) {
    data.optional_val = static_cast<uint8_t>(n);
  }
  [[nodiscard]] int sl() const {
    return data.sl;
  }
  void sl(int n) {
    data.sl = static_cast<uint8_t>(n);
  }
  [[nodiscard]] int dsl() const {
    return data.dsl;
  }
  void dsl(int n) {
    data.dsl = static_cast<uint8_t>(n);
  }
  [[nodiscard]] int exempt() const {
    return data.exempt;
  }
  void exempt(int n) {
    data.exempt = static_cast<uint8_t>(n);
  }
  [[nodiscard]] int raw_color(int n) const {
    if (n < 0 || n > 9) {
      return 7; // default color
    }
    return data.colors[n];
  }

  /**
   * Gets the color number n for this user.  Respects if ansi colors
   * is enabled or not.
   */
  [[nodiscard]] uint8_t color(int n) const { 
    if (n < 0 || n > 9) {
      return 7; // default color
    }
    return static_cast<uint8_t>(HasAnsi() ? raw_color(n) : bwcolor(n));
  }

  [[nodiscard]] std::vector<uint8_t> colors() const { 
    std::vector<uint8_t> c;
    for (auto i = 0; i < 10; i++) {
      if (HasColor()) {
        c.push_back(data.colors[i]);
      } else {
        c.push_back(data.bwcolors[i]);
      }
    }
    return c;
  }
  void color(int nColor, unsigned int n) {
    data.colors[nColor] = static_cast<uint8_t>(n);
  }
  [[nodiscard]] uint8_t bwcolor(int n) const {
    if (n < 0 || n > 9) {
      return 7; // default color
    }
    return data.bwcolors[n];
  }
  void bwcolor(int nColor, unsigned int n) {
    data.bwcolors[nColor] = static_cast<uint8_t>(n);
  }
  [[nodiscard]] int GetVote(int nVote) const {
    return data.votes[nVote];
  }
  void SetVote(int nVote, int n) {
    data.votes[nVote] = static_cast<uint8_t>(n);
  }
  [[nodiscard]] int illegal_logons() const {
    return data.illegal;
  }
  void illegal_logons(int n) {
    data.illegal = static_cast<uint8_t>(n);
  }
  void increment_illegal_logons() {
    if (data.illegal < std::numeric_limits<decltype(data.illegal)>::max()) {
      ++data.illegal;      
    }
  }
  [[nodiscard]] int email_waiting() const {
    return data.waiting;
  }
  void email_waiting(unsigned int n) {
    data.waiting = static_cast<uint8_t>(n);
  }
  [[nodiscard]] int ontoday() const {
    return data.ontoday;
  }
  /** 
   * Sets the birthday to month m (1-12), day d (1-31), 
   * year y (full 4 year date)
   */
  void birthday_mdy(int m, int d, int y);

  /** Gets the current user's birthday month (1-12) */
  [[nodiscard]] int birthday_month() const {
    return data.month;
  }

  /** Gets the current user's birthday day of the month (1-31) */
  [[nodiscard]] int birthday_mday() const {
    return data.day;
  }

  /** Gets the current user's birthday year (i.e. 1990) */
  [[nodiscard]] int birthday_year() const {
    return data.year + 1900;
  }

  /** Gets the current user's birthday as a DateTime */
  [[nodiscard]] core::DateTime birthday_dt() const;

  /** Gets the current user's birthday as a string in format "mm/dd/yy" */
  [[nodiscard]] std::string birthday_mmddyy() const;

  [[nodiscard]] int GetLanguage() const {
    return data.language;
  }
  void SetLanguage(int n) {
    data.language = static_cast<uint8_t>(n);
  }

  [[nodiscard]] int home_usernum() const {
    return data.homeuser;
  }
  void home_usernum(int n) {
    data.homeuser = static_cast<uint16_t>(n);
  }
  [[nodiscard]] uint16_t home_systemnum() const {
    return data.homesys;
  }
  void home_systemnum(uint16_t n) {
    data.homesys = n;
  }
  [[nodiscard]] uint16_t forward_usernum() const {
    return data.forwardusr;
  }
  void forward_usernum(uint16_t n) {
    data.forwardusr = n;
  }
  [[nodiscard]] uint16_t forward_systemnum() const {
    return data.forwardsys;
  }
  void forward_systemnum(uint16_t n) {
    data.forwardsys = n;
  }
  [[nodiscard]] int forward_netnum() const {
    return data.net_num;
  }
  void forward_netnum(int n) {
    data.net_num = static_cast<uint16_t>(n);
  }
  [[nodiscard]] int messages_posted() const {
    return data.msgpost;
  }
  void messages_posted(int n) {
    data.msgpost = static_cast<uint16_t>(n);
  }
  [[nodiscard]] int email_sent() const {
    return data.emailsent;
  }
  void email_sent(int n) {
    data.emailsent = static_cast<uint16_t>(n);
  }
  [[nodiscard]] int feedback_sent() const {
    return data.feedbacksent;
  }
  void feedback_sent(int n) {
    data.feedbacksent = static_cast<uint16_t>(n);
  }
  [[nodiscard]] int feedback_today() const {
    return data.fsenttoday1;
  }
  void feedback_today(int n) {
    data.fsenttoday1 = static_cast<uint16_t>(n);
  }
  [[nodiscard]] int posts_today() const {
    return data.posttoday;
  }
  void posts_today(int n) {
    data.posttoday = static_cast<uint16_t>(n);
  }
  [[nodiscard]] int email_today() const {
    return data.etoday;
  }
  void email_today(int n) {
    data.etoday = static_cast<uint16_t>(n);
  }
  [[nodiscard]] int ass_points() const {
    return data.ass_pts;
  }
  void ass_points(int n) {
    data.ass_pts = static_cast<uint16_t>(n);
  }
  void increment_ass_points(int n) {
    data.ass_pts = data.ass_pts + static_cast<uint16_t>(n);
  }
  [[nodiscard]] int uploaded() const {
    return data.uploaded;
  }
  void uploaded(int n) {
    data.uploaded = static_cast<uint16_t>(n);
  }
  void increment_uploaded() {
    ++data.uploaded;
  }
  void decrement_uploaded() {
    --data.uploaded;
  }
  [[nodiscard]] int downloaded() const {
    return data.downloaded;
  }
  void downloaded(int n) {
    data.downloaded = static_cast<uint16_t>(n);
  }
  void increment_downloaded() {
    ++data.downloaded;
  }
  void decrement_downloaded() {
    --data.downloaded;
  }
  [[nodiscard]] uint16_t last_bps() const {
    return data.lastrate;
  }
  void last_bps(int n) {
    data.lastrate = static_cast<uint16_t>(n);
  }
  [[nodiscard]] uint16_t logons() const {
    return data.logons;
  }
  void increment_logons() { ++data.logons; }

  void logons(int n) {
    data.logons = static_cast<uint16_t>(n);
  }
  [[nodiscard]] uint16_t email_net() const {
    return data.emailnet;
  }
  void email_net(uint16_t n) {
    data.emailnet = n;
  }
  [[nodiscard]] uint16_t posts_net() const {
    return data.postnet;
  }
  void posts_net(uint16_t n) {
    data.postnet = n;
  }
  [[nodiscard]] uint16_t deleted_posts() const {
    return data.deletedposts;
  }
  void deleted_posts(uint16_t n) {
    data.deletedposts = n;
  }
  [[nodiscard]] uint16_t chains_run() const {
    return data.chainsrun;
  }
  void chains_run(uint16_t n) {
    data.chainsrun = n;
  }
  [[nodiscard]] uint16_t gfiles_read() const {
    return data.gfilesread;
  }
  void gfiles_read(uint16_t n) {
    data.gfilesread = n;
  }
  [[nodiscard]] uint16_t banktime_minutes() const {
    return data.banktime;
  }
  void banktime_minutes(uint16_t n) {
    data.banktime = n;
  }
  void add_banktime_minutes(int n) {
    data.banktime += static_cast<uint16_t>(n);
  }
  void subtract_banktime_minutes(int n) {
    data.banktime -= static_cast<uint16_t>(n);
  }
  [[nodiscard]] uint16_t home_netnum() const {
    return data.homenet;
  }
  void home_netnum(uint16_t n) {
    data.homenet = n;
  }
  [[nodiscard]] uint16_t GetLastSubConf() const {
    return data.subconf;
  }
  void SetLastSubConf(uint16_t n) {
    data.subconf = n;
  }
  [[nodiscard]] uint16_t GetLastDirConf() const {
    return data.dirconf;
  }
  void SetLastDirConf(uint16_t n) {
    data.dirconf = n;
  }
  [[nodiscard]] uint16_t GetLastSubNum() const {
    return data.subnum;
  }
  void SetLastSubNum(uint16_t n) {
    data.subnum = n;
  }
  [[nodiscard]] uint16_t GetLastDirNum() const {
    return data.dirnum;
  }
  void SetLastDirNum(uint16_t n) {
    data.dirnum = n;
  }

  [[nodiscard]] uint32_t messages_read() const {
    return data.msgread;
  }
  void messages_read(uint32_t l) {
    data.msgread = l;
  }
  [[nodiscard]] uint32_t uk() const {
    return data.uk;
  }
  void set_uk(uint32_t l) {
    data.uk = l;
  }
  [[nodiscard]] uint32_t dk() const {
    return data.dk;
  }
  void set_dk(uint32_t l) {
    data.dk = l;
  }
  [[nodiscard]] daten_t last_daten() const {
    return data.daten;
  }
  void last_daten(daten_t l) {
    data.daten = l;
  }
  [[nodiscard]] uint32_t wwiv_regnum() const {
    return data.wwiv_regnum;
  }
  void wwiv_regnum(uint32_t l) {
    data.wwiv_regnum = l;
  }
  [[nodiscard]] daten_t nscan_daten() const {
    return data.datenscan;
  }
  void nscan_daten(daten_t l) {
    data.datenscan = l;
  }

  /** Adds extra time to the user, returns the new total extra time. */
  std::chrono::seconds add_extratime(std::chrono::duration<double> extra);
  
  /** Subtracts extra time to the user, returns the new total extra time. */
  std::chrono::seconds subtract_extratime(std::chrono::duration<double> extra);

  /** Total extra time awarded to this user */
  [[nodiscard]] std::chrono::duration<double> extra_time() const noexcept;
  
  /** Time online in total, in seconds */
  [[nodiscard]] std::chrono::seconds timeon() const;
  
  /** Time online today, in seconds */
  [[nodiscard]] std::chrono::seconds timeontoday() const;
  
  /** Add 'd' to the time online in total */
  std::chrono::seconds add_timeon(std::chrono::duration<double> d);
  
  /** Add 'd' to the time online for today */
  std::chrono::seconds add_timeon_today(std::chrono::duration<double> d);

  /** Returns the time on as seconds. */
  [[nodiscard]] float gold() const {
    return data.gold;
  }
  void gold(float f) {
    data.gold = f;
  }

  [[nodiscard]] bool GetFullFileDescriptions() const {
    return data.full_desc ? true : false;
  }
  void SetFullFileDescriptions(bool b) {
    data.full_desc = b ? 1 : 0;
  }

  [[nodiscard]] bool IsMailboxClosed() const { return forward_usernum() == 65535; }

  void CloseMailbox() {
    forward_systemnum(0);
    forward_usernum(65535);
  }
  [[nodiscard]] bool IsMailForwardedToInternet() const {
    return forward_usernum() == INTERNET_EMAIL_FAKE_OUTBOUND_NODE;
  }
  [[nodiscard]] bool IsMailboxForwarded() const {
    return forward_usernum() > 0 && forward_usernum() < INTERNET_EMAIL_FAKE_OUTBOUND_NODE;
  }
  void SetForwardToInternet() {
    forward_systemnum(INTERNET_EMAIL_FAKE_OUTBOUND_NODE);
  }
  void ClearMailboxForward() {
    forward_systemnum(0);
    forward_usernum(0);
  }

  /** Sets the last IP address from which this user connected. */
  void last_address(const core::ip_address& a) { data.last_address = a; }

  /** Returns the last IP address from which this user connected. */
  [[nodiscard]] core::ip_address last_address() const { return data.last_address; }

  /**
   * Creates a random password in the user's password field.
   */
  bool CreateRandomPassword();

  /** Should this user use hotkeys? */
  [[nodiscard]] bool hotkeys() const;
  void set_hotkeys(bool enabled);

  /** The current menu set for the user */
  [[nodiscard]] std::string menu_set() const;
  
  /** Sets the current menu set */
  void set_menu_set(const std::string& menu_set);

  ///////////////////////////////////////////////////////////////////////////
  // Static Helper Methods

  /** Creates a new user record in 'u' using the default values passed */
  static bool CreateNewUserRecord(User* u,
    uint8_t sl, uint8_t dsl, uint16_t restr, float gold,
    const std::vector<uint8_t>& newuser_colors,
    const std::vector<uint8_t>& newuser_bwcolors);

};

/** Reset today's user statistics for user u */
void ResetTodayUserStats(User* u);

/** Increments both the total calls and today's call stats for user 'u' */
int AddCallToday(User* u);

/** Returns the age in years for user 'u' */
int years_old(const User* u, core::Clock& clock);

}  // namespace wwiv::sdk

#endif