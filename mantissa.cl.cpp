////////////////////////////////////////////////////////////
#include "items.cl.hpp"
#include "../roengl/touch.hpp"
#include "../roengl/keyceptor.hpp"
#include "../basegame/basegame.hpp"
#include "../roengl/utils.hpp"
#include "../imgui/imlet.hpp"
#include "../roengl/roengl.hpp"
#include "../shing/easyparser.hpp"
#include "../shing/ripcurl.hpp"
#include "../shing/filevar.hpp"
#include "mantissa.cl.hpp"
#include "mantissa.ceeferinc.cl.hpp"
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
namespace Mantissa{
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
Manti::Manti(){
   key_input_textfield = client_key.val();
   secret_input_textfield = client_secret.val();
   command_center_name = command_hq_name.val();
   demo_phone_number = demomode_phonenum.val();
   setupKeys();
   setupImlets();
}
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
vector<Tag*> Manti::findTags(const string& thetext){
   vector<Tag*> ret;
   sloopmapex(tags, eh, moo)
      if(Morestring::contains(moo.name, thetext)){
         ret.push_back(&moo);
      }
   sloopend
   return ret;
}
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void Manti::syncContactSel(){
   if(selected_vols_synced){
      return;
   }
   selected_volunteers.clear();
   sloopmapex(volunteers, eh, moo)
      if(moo.selected){
         selected_volunteers.push_back(&moo); // easy
      }
   sloopend
   selected_vols_synced = true;
}
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void Manti::syncContacts(){
   if(volunteer_filter == last_applied_vol_filter && volunteers_synced){
      return;
   }
   sorted_volunteers.clear();
   auto sp = searchPred(volunteer_filter);
   sloopmapex(volunteers, eh, moo)
      moo.sync();
      if(sp.matches(moo)){
         sorted_volunteers.push_back(&moo);
      }
   sloopend
   sort(sorted_volunteers.begin(), sorted_volunteers.end(), [](auto * a, auto * b){
      return a->date > b->date;
   });
   last_applied_vol_filter = volunteer_filter;
   volunteers_synced = true;
}
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void Manti::syncTasks(){
   if(tasks_filter == last_applied_tasks_filter && tasks_synced){
      return;
   }
   sorted_tasks.clear();
   auto sp = searchPred(tasks_filter);
   sloopmapex(tasks, eh, moo)
      if(sp.matches(moo)){
         sorted_tasks.push_back(&moo);
      }
   sloopend
   sort(sorted_tasks.begin(), sorted_tasks.end(), [](auto * a, auto * b){
      return a->last_update > b->last_update;
   });
   last_applied_tasks_filter = tasks_filter;
   tasks_synced = true;
}
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void Manti::syncMessages(){
   if(message_filter == last_applied_message_filter && message_synced){
      return;
   }
   sorted_messages.clear();
   auto sp = searchPred(message_filter);
   sloopmapex(messages, eh, moo)
      moo.sync();
      if(sp.matches(moo)){
         sorted_messages.push_back(&moo);
      }
   sloopend
   sort(sorted_messages.begin(), sorted_messages.end(), [](auto * a, auto * b){
      return a->date > b->date;
   });
   last_applied_message_filter = message_filter;
   message_synced = true;
}
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
SearchPredPart Manti::searchPredPart(const string& thetext){
   SearchPredPart spp;
   spp.text = thetext;
   sloopmapex(tags, eh, moo)
      if(Morestring::contains(moo.name, thetext)){
         spp.tags.push_back(&moo);
      }
   sloopend
   return spp;
}
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
SearchPred Manti::searchPred(const string& thetext){
   SearchPred ret;
   EasyParser parsa(thetext);
   while(parsa--.hasChar()){
      if(parsa == '\"' || parsa == '\''){
         string literal = parsa.parseStringLiteralSmart();
         ret.parts.push_back(searchPredPart(literal));
      }
      else {
         string toget = parsa.parseUntil(' ', false);
         ret.parts.push_back(searchPredPart(toget));
      }
   }
   return ret;
}
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void Manti::unsyncOauth(){
   oauth_token = "";
   has_oauth_token_xnl = false;
   auth_token_tries_count = 0;
   has_oauth_error_xnl = false;
   croust<<"Unsyncedo auth!" << ozendl;
}
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void Manti::openRawMessagesList(){
   Imgui::curimo().openModal("rawmsgs" ) / [this] (auto& moda){
      Imgui::curimo().pushfont(3 ) / [this]{
         Imgui::curimo().text("Raw Inbound/Outbound Messages");
      };
      Imgui::curimo().sep();
      Imgui::curimo().text("Total Message Count : " );ImGui::SameLine();
      Imgui::curimo().text(__toString(raw_messages.size()) ,33_xcoel);
      Imgui::curimo().sep();
      Imgui::curimo().scrollRegion("items", -1, _roen->viewheight - 120)  / [&] {
         sloopveccounta(raw_messages, moo, counta)
            if(counta != 0){
               Imgui::curimo().sep();
            }
            if(moo.inbound){
               Imgui::curimo().text("from " ,33_xcoel );ImGui::SameLine();
               Imgui::curimo().text(moo.out_addr);
            }
            else {
               Imgui::curimo().text("to " ,112_xcoel );ImGui::SameLine();
               Imgui::curimo().text(moo.out_addr);
            }
            Imgui::curimo().textWrapped(moo.message);
            Imgui::curimo().text(moo.received_time.secDiffString() ,240_xcoel);
         sloopend
      };
      Imgui::curimo().sep();
      if(Imgui::curimo().button("done")){
         Imgui::curimo().closePopup();
      }
   };
}
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void Manti::openSettings(){
   croust<<"Opening settings!" << ozendl;
   Imgui::curimo().openModal("settings" ) / [this] (auto& moda){
      Imgui::curimo().sep();
      Imgui::curimo().pushfont(3 ) / [this]{
         Imgui::curimo().text("Setup Credentials");
      };
      if(last_oauth_error.size() != 0){
         if(ImGui::TreeNode("last oauth error")){
            Imgui::curimo().text("last oauth error " ,196_xcoel );ImGui::SameLine();
            Imgui::curimo().text(oauth_error_date.secDiffString() ,240_xcoel);
            ImGui::TextWrapped("%s" ,last_oauth_error.c_str());
            Imgui::curimo().sep();
            ImGui::TreePop();
         }
      }
      Imgui::curimo().text("First, you need to enter your TelstraDev SMS API");
      Imgui::curimo().text("secret/key to get an oauth token.");
      Imgui::curimo().inputText("client key" ,key_input_textfield ,200);
      Imgui::curimo().inputText("client secret " ,secret_input_textfield ,200);
      if(key_input_textfield != client_key.val()){
         client_key = key_input_textfield;
         unsyncOauth();
      }
      if(secret_input_textfield != client_secret.val()){
         client_secret = secret_input_textfield;
         unsyncOauth();
      }
      Imgui::curimo().sep();
      Imgui::curimo().pushfont(3 ) / [this]{
         Imgui::curimo().text("App Settings");
      };
      Imgui::curimo().inputText("name for broadcast" ,command_center_name ,40);
      if(command_hq_name.val() != command_center_name){
         command_hq_name = command_center_name;
      }
      Imgui::curimo().inputText("demo phone number" ,demo_phone_number ,40);
      if(demomode_phonenum.val() != demo_phone_number){
         demomode_phonenum = demo_phone_number;
      }
      if(demo_phone_number.size() != 0){
         Imgui::curimo().textWrapped("Demo Mode enabled - won't send to numbers other than the one listed");
      }
      Imgui::curimo().sep();
      if(Imgui::curimo().button("done")){
         Imgui::curimo().closePopup();
      }
   };
}
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void Manti::openContactsImportMenu(){
   contacts_import_buildup = "";
   contacts_textbox_focus = 30;
   Imgui::curimo().openModal("import" ) / [this] (auto& moda){
      Imgui::curimo().pushfont(3 ) / [this]{
         Imgui::curimo().text("Import Contacts");
      };
      Imgui::curimo().sep();
      Imgui::curimo().textWrapped("Enter contacts, line by line. First name, then followed by phone number, followed by tags/notes for filtering");
      Imgui::curimo().text("for example:" ,33_xcoel);
      Imgui::curimo().text("Elon Musk +61412345678 engineer ceo");
      Imgui::curimo().text("Alexander Mahone 0452 322 392 ex-fbi detective");
      Imgui::curimo().multiLineInput("contacts" ,contacts_import_buildup ,400 ,-1 ,10);
      Imgui::curimo().maybeFocusKeyboardHere(contacts_textbox_focus);
      Imgui::curimo().sep();
      Imgui::curimo().pushfont(2);
         if(Imgui::curimo().button("import")){
            croust<<"Importing" << ozendl;
            importContacts(contacts_import_buildup);
            Imgui::curimo().closePopup();
         }
         Imgui::curimo().sameLine();
         if(Imgui::curimo().button("cancel")){
            Imgui::curimo().closePopup();
         }
   };
}
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void Manti::importContacts(const string& masstext){
   vector<string> lines;
   __splitOn(masstext, '\n', lines);
   sloopvec(lines, moo)
      EasyParser parsa(moo);
      parsa--;
      int name_start = parsa.currentPoint();
      int phone_point = -1;
      while(parsa--.hasChar()){
         if(parsa.peekingAtPhoneNumber()){
            string name = __chompends(parsa.getShringerFromTo(name_start, parsa.currentPoint()).toString());
            auto phonenum = parsa.parsePhoneNumber();
            string notes = __chompends(parsa.parseUntilEnd());
            EasyParser nameparsa(name);
            string firstname = nameparsa.parseNameWord(); // yeah, sounds good
            createContact(firstname, name, phonenum, notes);
         }
         else {
            parsa.getChar();
         }
      }
   sloopend
}
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void Manti::createContact(const string& firstname, const string& fullname, Shing::PhoneNumber lenum, const string& lenotes){
   string numero;
   if(lenum.has_plus){
      numero = "+" + lenum.numbers;
   } else {
      numero = lenum.numbers;
   }
   if(Morestring::beginsWith(numero, "04")){
      numero = "+61" + Morestring::cutBegin(numero, "0");
   }
   auto& lecontact = volunteers[numero];
   lecontact.full_name = fullname;
   lecontact.firstname = firstname;
   lecontact.notes = lenotes;
   lecontact.save();
   volunteers_synced = false;
   selected_vols_synced = false;
}
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void Manti::openBroadcastMenu(bool foreverybody, bool forselected, vOLUNTEER letarg){
   croust<<"Opening broadcast menu!" << ozendl;
   broadcast_everybody = foreverybody;
   broadcast_selected_only = forselected;
   broadcast_targetuser = letarg;
   broadcast_message_buildup = "";
   broadcast_textbox_focus = 30;
   Imgui::curimo().openModal("broadcast" ) / [this] (auto& moda){
      Imgui::curimo().pushfont(3 ) / [this]{
         if(broadcast_targetuser){
            Imgui::curimo().text("Direct Message");
         }
         else {
            Imgui::curimo().text("Broadcast Message");
         }
      };
      Imgui::curimo().sep();
      Imgui::curimo().text("Message");
      Imgui::curimo().multiLineInput("message" ,broadcast_message_buildup ,400 ,-1 ,5);
      Imgui::curimo().maybeFocusKeyboardHere(broadcast_textbox_focus);
      Imgui::curimo().text(__toString(broadcast_message_buildup.size()) + " chars", 33_xcoel);
      Imgui::curimo().sep();
      if(broadcast_everybody){
         Imgui::curimo().text("Broadcasting for everybody..");
      }
      if(broadcast_selected_only){
         Imgui::curimo().text("Broadcasting to selected users:" ,220_xcoel);
         sloopvec(selected_volunteers, moo)
            Imgui::curimo().text(moo->full_name);
         sloopend
      }
      if(broadcast_targetuser){
         Imgui::curimo().text("Sending message to " );ImGui::SameLine();
         Imgui::curimo().text(broadcast_targetuser->full_name ,33_xcoel);
      }
      Imgui::curimo().sep();
      Imgui::curimo().pushfont(2);
         if(has_oauth_token_xnl() && has_provisional_number_xnl()){
            if(Imgui::curimo().button("send")){
               performBroadcast();
               Imgui::curimo().closePopup();
            }
         }
         else {
            Imgui::curimo().button("can't send yet" ,ColorInfo(0.12f));
            Imgui::curimo().tooltip("we don't have a provisional number yet");
         }
         Imgui::curimo().sameLine();
         if(Imgui::curimo().button("cancel")){
            Imgui::curimo().closePopup();
         }
   };
}
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void Manti::performBroadcast(){
   if(has_oauth_token_xnl() && has_provisional_number_xnl()){
   }
   else {
      return; // can't do jack shit here
   }
   vector<string> recipient_list;
   if(broadcast_everybody){
      sloopmapex(volunteers, eh, moo)
         recipient_list.push_back(moo.name);
      sloopend
   }
   else if(broadcast_selected_only){
      sloopvec(selected_volunteers, moo)
         recipient_list.push_back(moo->name); // name is the number I guess
      sloopend
   }
   else if(broadcast_targetuser){
      recipient_list.push_back(broadcast_targetuser->name); // just that number thanks
   }
   sloopvec(recipient_list, moo)
      performMessageSending(moo, broadcast_message_buildup);
   sloopend
   auto& mo = createNewMessageObject();
   mo.is_outbound = true;
   mo.outbound_numbers = recipient_list;
   mo.message = broadcast_message_buildup; // I guess that's that
   mo.save();
   message_synced = false;
}
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void Manti::rebroadcast(Message& lemessage){
   vector<string> recipient_list;
   string used_message = lemessage.message;
   if(Morestring::beginsWith(used_message, "REBROADCAST")){
   } else {
      used_message = "REBROADCAST:\n" + lemessage.message;
   }
   sloopmapex(volunteers, eh, moo)
      if(moo.name != lemessage.inbound_number){
         recipient_list.push_back(moo.name);
      }
   sloopend
   sloopvec(recipient_list, moo)
      performMessageSending(moo, used_message);
   sloopend
   auto& mo = createNewMessageObject();
   mo.is_outbound = true;
   mo.outbound_numbers = recipient_list;
   mo.message = used_message; // I guess that's that
   mo.save();
   message_synced = false;
}
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void Manti::performMessageSending(const string& target_number, const string& messagebui){
   RawMessage rm;
   rm.outbount = true;
   rm.out_addr = target_number;
   rm.message = messagebui;
   bool demomode = demo_phone_number.size() != 0;
   if(demomode && target_number != demo_phone_number){
      rm.not_actually_sent = true;
   }
   else {
      ripcurling("https://tapi.telstra.com/v2/messages/sms")
         .json_oauth(oauth_token)
         ("to", target_number)
         ("from", provisional_number) // source address that'll be displayed on the receiving device
         ("body", messagebui)
         ("validity", 125) // minutes to keep trying to send the message
         / [](auto& res){
            croust<<"Message sent!" << ozendl;
            res.print();
         };
   }
   raw_messages.push_front(rm);
}
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void Manti::openNewTaskDialogue(){
   croust<<"New task.." << ozendl;
   task_description_focus = 30;
   Imgui::curimo().openModal("new task" ) / [this] (auto& moda){
      Imgui::curimo().pushfont(3 ) / [this]{
         Imgui::curimo().text("New Task");
      };
      Imgui::curimo().sep();
      Imgui::curimo().text("you can assign volunteers to tasks.");
      Imgui::curimo().text("they'll get notified of the task, and so on.");
      Imgui::curimo().sep();
      Imgui::curimo().text("description");
      Imgui::curimo().multiLineInput("description" ,task_description ,600 ,-1 ,5);
      Imgui::curimo().maybeFocusKeyboardHere(task_description_focus);
      Imgui::curimo().sep();
      Imgui::curimo().text("notes");
      Imgui::curimo().multiLineInput("notes" ,task_notes ,600 ,-1 ,5);
      Imgui::curimo().text("note: task notes is not visible to volunteers");
      Imgui::curimo().sep();
      if(Imgui::curimo().button("create task")){
         createNewTask(task_description, task_notes);
         Imgui::curimo().closePopup();
      }
      Imgui::curimo().sameLine();
      if(Imgui::curimo().button("cancel")){
         Imgui::curimo().closePopup();
      }
   };
}
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void Manti::openTaskEdit(const string& taskname){
   currently_edited_task = &tasks[taskname];
   task_description = currently_edited_task->description;
   task_notes = currently_edited_task->notes;
   task_description_focus = 30;
   Imgui::curimo().openModal("edittask" ) / [this] (auto& moda){
      Imgui::curimo().text("Note: this thing is still under construction" ,196_xcoel);
      asserts(currently_edited_task, "")
      Imgui::curimo().pushfont(3 ) / [this]{
         Imgui::curimo().text("Edit Task");
      };
      Imgui::curimo().sep();
      Imgui::curimo().text("description");
      Imgui::curimo().multiLineInput("description" ,task_description ,600 ,-1 ,5);
      Imgui::curimo().sep();
      Imgui::curimo().text("notes");
      Imgui::curimo().multiLineInput("notes" ,task_notes ,600 ,-1 ,5);
      Imgui::curimo().maybeFocusKeyboardHere(task_description_focus);
      Imgui::curimo().text("note: task notes is not visible to volunteers");
      Imgui::curimo().sep();
      Imgui::curimo().sep();
      if(Imgui::curimo().button("mark finished")){
         Imgui::curimo().closePopup();
      }
      Imgui::curimo().sameLine();
      if(Imgui::curimo().button("cancel")){
         Imgui::curimo().closePopup();
      }
   };
}
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void Manti::openHelp(){
   croust<<"Opening help" << ozendl;
   Imgui::curimo().openModal("edittask" ) / [this] (auto& moda){
      Imgui::curimo().pushfont(3 ) / [this]{
         Imgui::curimo().text("Help");
      };
      Imgui::curimo().textWrapped("This software is under construction. Really, tried to do as much as possible in 1.5 days, but only managed to make a demo prototype.");
      Imgui::curimo().textWrapped("Having said that, the prototype functions work!");
      Imgui::curimo().sep();
      Imgui::curimo().text("Get in touch with developer:");
      Imgui::curimo().pushfont(2 ) / [this]{
         Imgui::curimo().text("ywid062@gmail.com" ,196_xcoel);
         Imgui::curimo().text("Yose Widjaja" ,220_xcoel);
      };
      if(Imgui::curimo().button("close")){
         Imgui::curimo().closePopup();
      }
   };
}
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void Manti::openVolunteerPopup(const string& volname){
   currently_popping_vol = &volunteers[volname];
   Imgui::curimo().openPopup("volpopup" ) / [this] (auto& moda){
      if(Imgui::curimo().button("message")){
         want_message_vol = currently_popping_vol;
         Imgui::curimo().closePopup();
      }
      Imgui::curimo().sameLine();
      Imgui::curimo().helpMarker("send a message to this user");
      if(Imgui::curimo().button("select")){
         Imgui::curimo().closePopup();
      }
      Imgui::curimo().sameLine();
      Imgui::curimo().helpMarker("select the item");
      if(Imgui::curimo().button("edit")){
         Imgui::curimo().closePopup();
      }
      Imgui::curimo().sameLine();
      Imgui::curimo().helpMarker("edit the message");
   };
}
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void Manti::openMessageContextMenu(const string& lemessageid){
   currently_popping_message = &messages[lemessageid];
   Imgui::curimo().openPopup("messagepopup" ) / [this] (auto& moda){
      if(Imgui::curimo().button("rebroadcast")){
         if(currently_popping_message){
            rebroadcast(currently_popping_message.ref());
         }
         Imgui::curimo().closePopup();
      }
      Imgui::curimo().sameLine();
      Imgui::curimo().helpMarker("this will rebroadcast the message to everybody");
      if(Imgui::curimo().button("delete")){
         if(currently_popping_message){
            deleteMessage(currently_popping_message.ref());
         }
         Imgui::curimo().closePopup();
      }
      Imgui::curimo().sameLine();
      Imgui::curimo().helpMarker("this will hide the message, but yeah");
      if(currently_popping_message && currently_popping_message->is_outbound){
         currently_popping_message->sync();
         Imgui::curimo().sep();
         Imgui::curimo().text("Recipients:");
         sloopvec(currently_popping_message->broadcast_targets, moo)
            Imgui::curimo().text(moo->full_name ,33_xcoel);
         sloopend
         sloopvec(currently_popping_message->unknown_broadcast_targets, moo)
            Imgui::curimo().text(moo ,240_xcoel);
         sloopend
      }
   };
}
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void Manti::deleteMessage(Message& lemessage){
   lemessage.eraseSelf();
   message_synced = false;
}
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void Manti::messagesPaneImcode(){
   Imgui::curimo().pushfont(2 ) / [this]{
      Imgui::curimo().text("Messages");
   };
   Imgui::curimo().sameLine();
   if(Imgui::curimo().button("broadcast")){
      openBroadcastMenu(true, false, NULL);
   }
   Imgui::curimo().sameLine();
   if(selected_volunteers.size() != 0){
      if(Imgui::curimo().button("send")){
         openBroadcastMenu(false, true, NULL);
      }
   }
   Imgui::curimo().sameLine();
   if(Imgui::curimo().button("logs")){
      openRawMessagesList();
   }
   Imgui::curimo().sep();
   Imgui::curimo().inputText("filter" ,message_filter ,150);
   Imgui::curimo().sep();
   syncMessages();
   string message_to_popup;
   Imgui::curimo().scrollRegion("items", -1, _roen->viewheight - 96)  / [&] {
      sloopveccounta(sorted_messages, moo, counta)
         if(counta != 0){
            Imgui::curimo().sep();
         }
         Imgui::curimo().idpush(counta ) / [&] {
            moo->sync();
            bool was_clicked = false;
            if(moo->is_inbound){
               Imgui::curimo().text("from" ,33_xcoel );ImGui::SameLine();
               was_clicked = was_clicked || Imgui::curimo().clicked();
               Imgui::curimo().text(moo->getInboundLabel());
               was_clicked = was_clicked || Imgui::curimo().clicked();
            }
            else {
               if(moo->outbound_numbers.size() == 1){
                  Imgui::curimo().text("message to" ,112_xcoel );ImGui::SameLine();
                  was_clicked = was_clicked || Imgui::curimo().clicked();
                  if(moo->broadcast_targets.size() != 0){
                     Imgui::curimo().text(moo->broadcast_targets[0]->full_name);
                  }
                  else if(moo->unknown_broadcast_targets.size() != 0){
                     Imgui::curimo().text(moo->unknown_broadcast_targets[0]);
                  }
                  was_clicked = was_clicked || Imgui::curimo().clicked();
               }
               else {
                  Imgui::curimo().text("broadcast" ,112_xcoel );ImGui::SameLine();
                  was_clicked = was_clicked || Imgui::curimo().clicked();
                  Imgui::curimo().text (__toString(moo->outbound_numbers.size()) + " recipients");
                  was_clicked = was_clicked || Imgui::curimo().clicked();
               }
            }
            Imgui::curimo().textWrapped(moo->message);
            was_clicked = was_clicked || Imgui::curimo().clicked();
            Imgui::curimo().text(moo->date.secDiffString() ,240_xcoel);
            was_clicked = was_clicked || Imgui::curimo().clicked();
            if(was_clicked){
               message_to_popup = moo->name; // I guess by ID is alright
            }
         };
      sloopend
   };
   if(message_to_popup.size() != 0){
      openMessageContextMenu(message_to_popup);
   }
}
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void Manti::tasksPaneImcode(){
   Imgui::curimo().pushfont(2 ) / [this]{
      Imgui::curimo().text("Tasks");
   };
   Imgui::curimo().sameLine();
   if(Imgui::curimo().button("new task")){
      openNewTaskDialogue();
   }
   Imgui::curimo().sep();
   Imgui::curimo().inputText("filter" ,tasks_filter ,150);
   Imgui::curimo().sep();
   syncTasks();
   string task_to_edit;
   Imgui::curimo().scrollRegion("items", -1, _roen->viewheight - 96)  / [&] {
      sloopveccounta(sorted_tasks, moo, counta)
         if(counta != 0){
            Imgui::curimo().text("");
         }
         Imgui::curimo().idpush(counta ) / [&] {
            moo->sync();
            Imgui::curimo().textWrapped(moo->description ,220_xcoel);
            Imgui::curimo().sep();
            Imgui::curimo().text("Last update " ,240_xcoel );ImGui::SameLine();
            Imgui::curimo().text(moo->last_update.secDiffString() ,240_xcoel);
            Imgui::curimo().sameLine();
            if(Imgui::curimo().button("edit")){
               task_to_edit = moo->name;
            }
            if(selected_volunteers.size() != 0){
               Imgui::curimo().sameLine();
               if(Imgui::curimo().button("assign")){
                  assignSelectedVolunteersTo(*moo); // I guess
               }
               Imgui::curimo().tooltip("Assign those contacts into this task, notifying them while at it");
            }
            else {
               Imgui::curimo().sameLine();
               Imgui::curimo().button("assign" ,ColorInfo(0.2f));
               Imgui::curimo().tooltip("select contacts first to assign to this task");
            }
            if(moo->assignments.size() != 0){
               Imgui::curimo().sameLine();
               if(Imgui::curimo().button("unassign")){
                  clearTaskAssignment(*moo);
               }
               Imgui::curimo().tooltip("Unassign from this task");
            }
            Imgui::curimo().sep();
            if(moo->assignments.size() == 0){
               Imgui::curimo().text("nobody assigned to this task" ,240_xcoel);
            }
            else {
               sloopveccounta(moo->vol_assignees, assignee, counta)
                  if(counta >= 2){
                     int leftover = moo->vol_assignees.size() - counta;
                     Imgui::curimo().text ("and " + __toString(leftover) + " others assigned");
                     break;
                  }
                  Imgui::curimo().text(assignee->full_name );ImGui::SameLine();
                  Imgui::curimo().text("assigned" ,240_xcoel);
               sloopend
            }
         };
      sloopend
   };
   if(task_to_edit.size() != 0){
      openTaskEdit(task_to_edit);
   }
}
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void Manti::assignSelectedVolunteersTo(Task& letask){
   sloopvec(selected_volunteers, moo)
      letask.addVolunteer(moo, this);
   sloopend
   tasks_synced = false;
   sloopvec(selected_volunteers, moo)
      moo->selected = false;
   sloopend
   selected_vols_synced = false;
}
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void Manti::clearTaskAssignment(Task& letask){
   letask.clearVolunteers();
}
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void Manti::volunteersPaneImcode(){
   Imgui::curimo().pushfont(2 ) / [this]{
      Imgui::curimo().text("contacts");
   };
   Imgui::curimo().sameLine();
   if(Imgui::curimo().button("import contacts")){
      openContactsImportMenu();
   }
   Imgui::curimo().sep();
   Imgui::curimo().inputText("filter" ,volunteer_filter ,150);
   Imgui::curimo().sep();
   syncContacts();
   syncContactSel();
   string contact_to_popup;
   if(want_message_vol){
      openBroadcastMenu(false, false, want_message_vol);
      want_message_vol = NULL;
   }
   Imgui::curimo().scrollRegion("items", -1, _roen->viewheight - 96)  / [&] {
      sloopveccounta(sorted_volunteers, moo, counta)
         if(counta != 0){
            Imgui::curimo().sep();
         }
         Imgui::curimo().idpush(counta ) / [&] {
            moo->sync();
            if(!moo->selected){
               Imgui::curimo().text("SEL" ,240_xcoel );ImGui::SameLine();
               Imgui::curimo().tooltip("click to select this contact");
            }
            else {
               Imgui::curimo().text("SEL" ,112_xcoel );ImGui::SameLine();
               Imgui::curimo().tooltip("click to unselect this contact");
            }
            if(Imgui::curimo().clicked()){
               moo->selected = !moo->selected;
               selected_vols_synced = false;
            }
            bool was_clicked = false;
            if(moo->selected){
               Imgui::curimo().text(moo->full_name ,33_xcoel);
            }
            else {
               Imgui::curimo().text(moo->full_name);
            }
            was_clicked = was_clicked || Imgui::curimo().clicked();
            if(Imgui::curimo().isItemHovered()){
               Imgui::curimo().tooltip(moo->name);
            }
            Imgui::curimo().sameLine();
            Imgui::curimo().text(moo->notes ,240_xcoel);
            was_clicked = was_clicked || Imgui::curimo().clicked();
            if(moo->taskptrs.size() != 0){
               Imgui::curimo().text ("assigned to " + __toString(moo->taskptrs.size()) + " tasks", 244_xcoel);
               was_clicked = was_clicked || Imgui::curimo().clicked();
               Imgui::curimo().tooltip([&](auto& imo){
                  sloopvec(moo->taskptrs, letask)
                     Imgui::curimo().text(Morestring::firstN(letask->description, 40, ".."));
                  sloopend
               });
            }
            if(was_clicked){
               contact_to_popup = moo->name;
            }
         };
      sloopend
   };
   if(contact_to_popup.size() != 0){
      openVolunteerPopup(contact_to_popup);
   }
}
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void Manti::setupImlets(){
   int viewwidth = _roen->viewwidth;
   int panel_width = (viewwidth / 3) - 8; // 4 pixels on each side
   imlet("titler").noBackground().noScroll().fixedPane().posiRTL(4,4).width(200).height(44)  / [this] (auto& imo){
      if(command_hq_name.val().size() == 0){
         Imgui::curimo().text("SHADOW" ,33_xcoel );ImGui::SameLine();
         Imgui::curimo().text("MANTISSA" ,112_xcoel );ImGui::SameLine();
      }
      else {
         Imgui::curimo().text(command_hq_name.val() ,220_xcoel );ImGui::SameLine();
      }
      Imgui::curimo().text(" powered by TelstraDev SMS API");
      if(has_oauth_error_xnl()){
         if(Imgui::curimo().button("error detected, click here")){
            openSettings();
         }
      }
      else if(!has_oauth_token_xnl()){
         if(client_key->size() == 0 || client_secret->size() == 0){
            if(Imgui::curimo().button("setup client key/secret first!")){
               openSettings();
            }
         }
         else {
            Imgui::curimo().pushfont(2 ) / [this]{
               Imgui::curimo().text("obtaining oauth token.." );ImGui::SameLine();
               Imgui::curimo().asciiSpinner();
            };
            Imgui::curimo().text ("(attempt " + __toString(auth_token_tries_count) + ")", 33_xcoel);
         }
      }
      else {
         if(!has_provisional_number_xnl()){
            Imgui::curimo().pushfont(2 ) / [this]{
               Imgui::curimo().text("getting provi. number.." );ImGui::SameLine();
               Imgui::curimo().asciiSpinner();
            };
            Imgui::curimo().text ("(attempt " + __toString(phone_number_tries_count) + ")", 33_xcoel);
         }
         else {
            Imgui::curimo().pushfont(3) / [this]{
               Imgui::curimo().text(provisional_number ,33_xcoel);
            };
            if(last_succ_messsage_check.isNull()){
               Imgui::curimo().text("no messages check done yet");
            }
            else if(last_succ_messsage_check.hasPassed(30)){
               Imgui::curimo().text("last message check : " ,196_xcoel );ImGui::SameLine();
               Imgui::curimo().text(last_succ_messsage_check.secDiffString() );ImGui::SameLine();
               Imgui::curimo().text(" (too long ago)");
            }
            else if(!last_message_receive.isNull()){
               Imgui::curimo().text("last message : " );ImGui::SameLine();
               Imgui::curimo().text(last_message_receive.secDiffString() ,33_xcoel);
            }
            else {
               Imgui::curimo().text("no messages received yet");
            }
         }
      }
   };
   imlet("menuer").fixedPane().posiRTR(4,6).width(viewwidth - 200 - 8).height(26)  / [this] (auto& imo){
      Imgui::curimo().pushfont(2.0f ) / [this]{
         Imgui::curimo().sameLine();
         ImGui::AlignTextToFramePadding();
         if(Imgui::curimo().button("BROADCAST")){
            openBroadcastMenu(true, false, NULL);
         }
         Imgui::curimo().sameLine();
         ImGui::AlignTextToFramePadding();
         Imgui::curimo().text(" | " ,240_xcoel);
         Imgui::curimo().sameLine();
         ImGui::AlignTextToFramePadding();
         if(Imgui::curimo().button("IMPORT")){
            openContactsImportMenu();
         }
         Imgui::curimo().sameLine();
         ImGui::AlignTextToFramePadding();
         Imgui::curimo().text(" | " ,240_xcoel);
         Imgui::curimo().sameLine();
         ImGui::AlignTextToFramePadding();
         if(Imgui::curimo().button("SETTINGS")){
            openSettings();
         }
         Imgui::curimo().sameLine();
         ImGui::AlignTextToFramePadding();
         Imgui::curimo().text(" | " ,240_xcoel);
         Imgui::curimo().sameLine();
         ImGui::AlignTextToFramePadding();
         if(Imgui::curimo().button("SMS LOGS")){
            openRawMessagesList();
         }
         Imgui::curimo().sameLine();
         ImGui::AlignTextToFramePadding();
         Imgui::curimo().text(" | " ,240_xcoel);
         Imgui::curimo().sameLine();
         ImGui::AlignTextToFramePadding();
         if(Imgui::curimo().button("HELP")){
            openHelp();
         }
      };
   };
   imlet("messagespane").fixedPane().posiRTL(4,52).width(panel_width).heightRoenMinus(56)  / [this] (auto& imo){
      messagesPaneImcode();
   };
   imlet("tasklist").fixedPane().noBG().posiRTM(52).width(panel_width).heightRoenMinus(56)  / [this] (auto& imo){
      tasksPaneImcode();
   };
   imlet("volunteerlist").fixedPane().posiRTR(4,52).width(panel_width).heightRoenMinus(56)  / [this] (auto& imo){
      volunteersPaneImcode();
   };
}
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void Manti::setupKeys(){
   _keyceptor[0](rk_r)("R!") / [this](auto rkd){
      croust<<"R PRESSED - RESTARTING!" << ozendl;
      return true;
   };
}
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void Manti::update(){
   if(__roenupdp % 60 == 0){
      if(has_oauth_token_xnl() && oauth_token_expiry.hasPassed()){
         has_oauth_token_xnl = false;
      }
      if(!has_oauth_error_xnl() && !has_oauth_token_xnl() && client_key->size() != 0 && client_secret->size() != 0){
         if(auth_token_tries_count < 10){
            auth_token_tries_count++;
            ripcurling("https://tapi.telstra.com/v2/oauth/token")
               .urlencoded()
               ("client_id", client_key.val())
               ("client_secret", client_secret.val())
               ("grant_type", "client_credentials")
               ("scope", "NSMS")  / [this] (auto& res){
                  if(res){
                     has_oauth_error_xnl = false;
                     has_oauth_token_xnl = true;
                     oauth_token = res["access_token"];
                     int expirysecs = Morestring::toInt(res["expires_in"]);
                     oauth_token_expiry = SDate() + (expirysecs - 120);
                  }
                  else {
                     has_oauth_error_xnl = true;
                     last_oauth_error = res.contents;
                     oauth_error_date = SDate();
                  }
               };
         }
      }
      if(has_oauth_token_xnl() && !has_provisional_number_xnl()){
         if(phone_number_tries_count < 10){
            phone_number_tries_count++;
            ripcurling("https://tapi.telstra.com/v2/messages/provisioning/subscriptions")
               .json_oauth(oauth_token)
               ("activeDays", 30)  / [this] (auto& res){
                  if(!res){
                  }
                  else {
                     string destaddress = res["destinationAddress"];
                     if(destaddress.size() != 0){
                        has_provisional_number_xnl = true;
                        provisional_number = destaddress;
                     }
                  }
               };
         }
      }
      if(has_provisional_number_xnl() && has_oauth_token_xnl()){
         ripcurling("https://tapi.telstra.com/v2/messages/sms")
            .getmode()
            .json_oauth(oauth_token)  / [this] (auto& res){
               if(res){
                  if(res("message")){
                     string messageid = res["messageId"];
                     string message = res["message"];
                     string sender = res["senderAddress"];
                     receiveMessageRaw(messageid, message, sender);
                     last_message_receive = SDate();
                  }
                  last_succ_messsage_check = SDate();
               }
            };
      }
   }
}
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void Manti::receiveMessageRaw(const string& theid, const string& themessage, const string& sender){
   RawMessage rm;
   rm.inbound = true;
   rm.message_id = theid;
   rm.out_addr = sender;
   rm.message = themessage;
   raw_messages.push_front(rm); // start at the front, I guess
   onReceiveMessage(rm);
}
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
Message& Manti::createNewMessageObject(){
   SDate rightnow;
   static int message_counter = 0;
   message_counter++;
   string theid = __toString(rightnow.getUnixSeconds()) + ":" + __toString(message_counter);
   return messages[theid];
}
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void Manti::onReceiveMessage(RawMessage& rm){
   auto& lemess = createNewMessageObject();
   lemess.is_inbound = true;
   lemess.inbound_number = rm.out_addr;
   lemess.message = rm.message;
   lemess.date = rm.received_time; //. whatever
   lemess.save();
   message_synced = false;
}
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void Manti::drawPost2D(){
}
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
bool Manti::handleTouchEvent(const TouchInfo& tinfoil){
   return false;
}
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
Task& Manti::createNewTask(const string& thedesc, const string& thenotes){
   string taskid = __toString(SDate().getUnixSeconds()); // so hacky, lel
   auto& ret = tasks[taskid];
   ret.description = thedesc;
   ret.notes = thenotes;
   tasks_synced = false;
   ret.save();
   return ret;
}
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
string compactifyNumber(const string& thenum){
   string ret;
   sloopvec(thenum, moo)
      if(moo == ' '){
      }
      else {
         ret.append(1, moo);
      }
   sloopend
   return ret;
}
////////////////////////////////////////////////////////////
   CLOTHDECLpermbody(string,client_key,"mantissa.sqlite", string());
   CLOTHDECLpermbody(string,client_secret,"mantissa.sqlite", string());
   CLOTHDECLpermbody(string,command_hq_name,"mantissa.sqlite", string());
   CLOTHDECLpermbody(string,demomode_phonenum,"mantissa.sqlite", string());
////////////////////////////////////////////////////////////
}; // end namespace Mantissa
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
