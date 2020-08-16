////////////////////////////////////////////////////////////
#pragma once
namespace std{}; using namespace std;
////////////////////////////////////////////////////////////
#include "cloth_classdef.cl.hpp"
#include "../shing/schdangler.hpp"
#include "../lmdb/lmap.hpp"
#include "../shing/signol.hpp"
#include "../sqlite/perm.hpp"
#include "../shing/phonenumber.hpp"
#include "items.cl.hpp"
#include "../roengl/roengl_classdef.hpp"
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
namespace Mantissa{
class Manti{
   public:
   //-------------------------------------------------------
   string command_center_name;
   string demo_phone_number;
   SDate last_succ_messsage_check = SDate(0);
   SDate last_message_receive = SDate(0);
   string oauth_token;
   SDate oauth_token_expiry;
   Xnl has_oauth_token_xnl;
   Xnl has_oauth_error_xnl;
   string last_oauth_error;
   SDate oauth_error_date = SDate(0);
   int auth_token_tries_count = 0;
   int phone_number_tries_count = 0;
   Xnl has_provisional_number_xnl;
   string provisional_number;
   string key_input_textfield;
   string secret_input_textfield;
   int contacts_textbox_focus = 0;
   string contacts_import_buildup;
   int broadcast_textbox_focus = 0;
   string broadcast_message_buildup;
   mESSAGE currently_popping_message;
   bool broadcast_everybody = false;
   bool broadcast_selected_only = false;
   vOLUNTEER broadcast_targetuser;
   vOLUNTEER currently_popping_vol;
   int task_description_focus = 0;
   string task_description;
   string task_notes;
   tASK currently_edited_task;
   string message_filter;
   string last_applied_message_filter;
   vector<Message*> sorted_messages;
   bool message_synced = false;
   string tasks_filter;
   string last_applied_tasks_filter;
   vector<Task*> sorted_tasks;
   bool tasks_synced = false;
   string volunteer_filter;
   string last_applied_vol_filter;
   vector<Volunteer*> sorted_volunteers;
   bool volunteers_synced = false;
   vector<Volunteer*> selected_volunteers;
   bool selected_vols_synced = false;
   vOLUNTEER want_message_vol;
   deque<RawMessage> raw_messages;
   //-------------------------------------------------------
   Manti();
   vector<Tag*> findTags(const string& thetext);
   void syncContactSel();
   void syncContacts();
   void syncTasks();
   void syncMessages();
   SearchPredPart searchPredPart(const string& thetext);
   SearchPred searchPred(const string& thetext);
   void unsyncOauth();
   void openRawMessagesList();
   void openSettings();
   void openContactsImportMenu();
   void importContacts(const string& masstext);
   void createContact(const string& firstname, const string& fullname, Shing::PhoneNumber lenum, const string& lenotes);
   void openBroadcastMenu(bool foreverybody, bool forselected, vOLUNTEER letarg);
   void performBroadcast();
   void rebroadcast(Message& lemessage);
   void performMessageSending(const string& target_number, const string& messagebui);
   void openNewTaskDialogue();
   void openTaskEdit(const string& taskname);
   void openHelp();
   void openVolunteerPopup(const string& volname);
   void openMessageContextMenu(const string& lemessageid);
   void deleteMessage(Message& lemessage);
   void messagesPaneImcode();
   void tasksPaneImcode();
   void assignSelectedVolunteersTo(Task& letask);
   void clearTaskAssignment(Task& letask);
   void volunteersPaneImcode();
   void setupImlets();
   void setupKeys();
   void update();
   void receiveMessageRaw(const string& theid, const string& themessage, const string& sender);
   Message& createNewMessageObject();
   void onReceiveMessage(RawMessage& rm);
   void drawPost2D();
   bool handleTouchEvent(const TouchInfo& tinfoil);
   Task& createNewTask(const string& thedesc, const string& thenotes);
   //-------------------------------------------------------
};
////////////////////////////////////////////////////////////
string compactifyNumber(const string& thenum);
////////////////////////////////////////////////////////////
   CLOTHDECLpermhead(string,client_key);
   CLOTHDECLpermhead(string,client_secret);
   CLOTHDECLpermhead(string,command_hq_name);
   CLOTHDECLpermhead(string,demomode_phonenum);
DECLschdangler(manti, Manti);
}; // end namespace Mantissa
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
