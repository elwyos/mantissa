//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
#bodinc keyceptor basegame utils imgui.imlet roengl easyparser ripcurl
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
++/compactifyNumber(const string& thenum) -> string
   string ret;
   \sloop thenum
      \if moo == ' '
      \else
         ret.append(1, moo);
   
   return ret;
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
\settings "bundle/test.flvr"
   // eh.. what's here
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
Manti/Manti()
   
   key_input_textfield = client_key.val();
   secret_input_textfield = client_secret.val();
   command_center_name = command_hq_name.val();
   demo_phone_number = demomode_phonenum.val();
   
   // -------------------------- initialization -----------------------
   
   setupKeys();
   setupImlets();
   
   // -------------------------- initialization -----------------------
   
   
   
   // -------------------------- quick start -----------------------
   
   
   
//////////////////////////////////////////////////////////////////////////////////////////
Manti/findTags(const string& thetext) -> vector<Tag*>
   vector<Tag*> ret;
   \sloopmap tags
      \if Morestring::contains(moo.name, thetext)
         ret.push_back(&moo);
   
   return ret;
         
//////////////////////////////////////////////////////////////////////////////////////////
Manti/syncContactSel()
   \if selected_vols_synced
      return;
   
   selected_volunteers.clear();
   \sloopmap volunteers
      \if moo.selected
         selected_volunteers.push_back(&moo); // easy
   
   selected_vols_synced = true;
//////////////////////////////////////////////////////////////////////////////////////////
Manti/syncContacts()

   \if volunteer_filter == last_applied_vol_filter && volunteers_synced
      return;
   
   // depends on message_filter I guess
   
   sorted_volunteers.clear();
   
   // find applying tags
   auto sp = searchPred(volunteer_filter);
   
   \sloopmap volunteers
      // well
      
      moo.sync();
      // fuck, gotta sleep I guess
      \if sp.matches(moo)
         sorted_volunteers.push_back(&moo);
   
   
   sort(sorted_volunteers.begin(), sorted_volunteers.end(), [](auto * a, auto * b){
      return a->date > b->date;
      // latest topmost
   });
   
   last_applied_vol_filter = volunteer_filter;
   volunteers_synced = true;
   
//////////////////////////////////////////////////////////////////////////////////////////
Manti/syncTasks()
   
   \if tasks_filter == last_applied_tasks_filter && tasks_synced
      return;
   
   sorted_tasks.clear();
   // find applying tags
   auto sp = searchPred(tasks_filter);
   
   \sloopmap tasks
      \if sp.matches(moo)
         sorted_tasks.push_back(&moo);
   
   
   sort(sorted_tasks.begin(), sorted_tasks.end(), [](auto * a, auto * b){
      return a->last_update > b->last_update;
      // latest topmost
   });
   
   last_applied_tasks_filter = tasks_filter;
   tasks_synced = true;
   
//////////////////////////////////////////////////////////////////////////////////////////
Manti/syncMessages()
   
   \if message_filter == last_applied_message_filter && message_synced
      return;
   
   // depends on message_filter I guess
   
   sorted_messages.clear();
   
   
   // find applying tags
   auto sp = searchPred(message_filter);
   
   \sloopmap messages
      // well
      
      moo.sync();
      // fuck, gotta sleep I guess
      \if sp.matches(moo)
         sorted_messages.push_back(&moo);
   
   
   sort(sorted_messages.begin(), sorted_messages.end(), [](auto * a, auto * b){
      return a->date > b->date;
      // latest topmost
   });
   
   last_applied_message_filter = message_filter;
   message_synced = true;
   
//////////////////////////////////////////////////////////////////////////////////////////
Manti/searchPredPart(const string& thetext) -> SearchPredPart
   SearchPredPart spp;
   spp.text = thetext;
   \sloopmap tags
      \if Morestring::contains(moo.name, thetext)
         spp.tags.push_back(&moo);
         
   return spp;
//////////////////////////////////////////////////////////////////////////////////////////
Manti/searchPred(const string& thetext) -> SearchPred
   SearchPred ret;
   EasyParser parsa(thetext);
   \while parsa--.hasChar()
      \if parsa == '\"' || parsa == '\''
         string literal = parsa.parseStringLiteralSmart();
         ret.parts.push_back(searchPredPart(literal));
      \else
         string toget = parsa.parseUntil(' ', false);
         ret.parts.push_back(searchPredPart(toget));
   return ret;
   
//////////////////////////////////////////////////////////////////////////////////////////
Manti/unsyncOauth()
   oauth_token = "";
   has_oauth_token_xnl = false;
   auth_token_tries_count = 0;
   has_oauth_error_xnl = false;
   \\/ "Unsyncedo auth!"
//////////////////////////////////////////////////////////////////////////////////////////
Manti/openRawMessagesList()

   im\ openModal "rawmsgs" /moda/
      
      im\ pushfont 3 /
         im\/ "Raw Inbound/Outbound Messages"
      
      im\----------------------
      im\/ "Total Message Count : " -
      im\/ __toString(raw_messages.size()) 33_xcoel
      im\----------------------
      
      im\ scrollRegion("items", -1, _roen->viewheight - 120) /&/
         \sloop raw_messages, moo, counta
            \if counta != 0
               im\-------------------------
            
            // let's see
            \if moo.inbound
               im\/ "from " 33_xcoel -
               im\/ moo.out_addr
            \else
               im\/ "to " 112_xcoel -
               im\/ moo.out_addr
            
            im\ textWrapped moo.message
            im\/ moo.received_time.secDiffString() 240_xcoel
         
      im\-----------------
      
      \if im\ button "done"
         im\ closePopup
      
      
//////////////////////////////////////////////////////////////////////////////////////////
Manti/openSettings()
   \\/ "Opening settings!"
   
   im\ openModal "settings" /moda/
      
      im\-----------------
      
      im\ pushfont 3 /
         im\/ "Setup Credentials"
      
      
      \if last_oauth_error.size() != 0
         \if im\ TreeNode "last oauth error"
            im\/ "last oauth error " 196_xcoel -
            im\/ oauth_error_date.secDiffString() 240_xcoel
            im\ TextWrapped "%s" last_oauth_error.c_str()
            im\-------
            im\ TreePop();
      
      im\/ "First, you need to enter your TelstraDev SMS API"
      im\/ "secret/key to get an oauth token."
      
      im\ inputText "client key" key_input_textfield 200
      im\ inputText "client secret " secret_input_textfield 200
      
      \if key_input_textfield != client_key.val()
         client_key = key_input_textfield;
         unsyncOauth();
         
      \if secret_input_textfield != client_secret.val()
         client_secret = secret_input_textfield;
         unsyncOauth();
      
      
      im\-----------------
      im\ pushfont 3 /
         im\/ "App Settings"
      
      im\ inputText "name for broadcast" command_center_name 40
      \if command_hq_name.val() != command_center_name
         command_hq_name = command_center_name;
         
      im\ inputText "demo phone number" demo_phone_number 40
      \if demomode_phonenum.val() != demo_phone_number
         demomode_phonenum = demo_phone_number;
         
      \if demo_phone_number.size() != 0
         im\ textWrapped "Demo Mode enabled - won't send to numbers other than the one listed"
      
      im\-----------------
      
      \if im\ button "done"
         im\ closePopup
      
   
//////////////////////////////////////////////////////////////////////////////////////////
Manti/openContactsImportMenu()
   
   contacts_import_buildup = "";
   contacts_textbox_focus = 30;
   im\ openModal "import" /moda/
      im\ pushfont 3 /
         im\/ "Import Contacts"
      
      im\---------------------------------
      im\ textWrapped "Enter contacts, line by line. First name, then followed by phone number, followed by tags/notes for filtering"
      
      im\/ "for example:" 33_xcoel
      im\/ "Elon Musk +61412345678 engineer ceo"
      im\/ "Alexander Mahone 0452 322 392 ex-fbi detective"
      
      im\ multiLineInput "contacts" contacts_import_buildup 400 -1 10
      im\ maybeFocusKeyboardHere contacts_textbox_focus
      
      im\---------------------------------
      
      im\ pushfont 2
         \if im\ button "import"
            \\/ "Importing"
            importContacts(contacts_import_buildup);
            im\ closePopup
         
         im\ sameLine
         
         \if im\ button "cancel"
            im\ closePopup
//////////////////////////////////////////////////////////////////////////////////////////
Manti/importContacts(const string& masstext)
   vector<string> lines;
   __splitOn(masstext, '\n', lines);
   
   \sloop lines
      EasyParser parsa(moo);
      
      parsa--;
      int name_start = parsa.currentPoint();
      
      int phone_point = -1;
      
      \while parsa--.hasChar()
         \if parsa.peekingAtPhoneNumber()
         
            string name = __chompends(parsa.getShringerFromTo(name_start, parsa.currentPoint()).toString());
            
            auto phonenum = parsa.parsePhoneNumber();
            
            
            string notes = __chompends(parsa.parseUntilEnd());
            
            EasyParser nameparsa(name);
            string firstname = nameparsa.parseNameWord(); // yeah, sounds good
            createContact(firstname, name, phonenum, notes);
            
         \else
            parsa.getChar();
      
      
      
//////////////////////////////////////////////////////////////////////////////////////////
Manti/createContact(const string& firstname, const string& fullname, Shing::PhoneNumber lenum, const string& lenotes)
   
   // sanitize phone number
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
   
//////////////////////////////////////////////////////////////////////////////////////////
Manti/openBroadcastMenu(bool foreverybody, bool forselected, vOLUNTEER letarg)
   
   \\/ "Opening broadcast menu!"
   
   broadcast_everybody = foreverybody;
   broadcast_selected_only = forselected;
   broadcast_targetuser = letarg;
   broadcast_message_buildup = "";
   
   broadcast_textbox_focus = 30;
   im\ openModal "broadcast" /moda/
      im\ pushfont 3 /
         \if broadcast_targetuser
            im\/ "Direct Message"
         \else
            im\/ "Broadcast Message"
      
      im\-------------
      im\/ "Message"
      im\ multiLineInput "message" broadcast_message_buildup 400 -1 5
      im\ maybeFocusKeyboardHere broadcast_textbox_focus
      im\/(__toString(broadcast_message_buildup.size()) + " chars", 33_xcoel);
      im\-------------
      \if broadcast_everybody
         im\/ "Broadcasting for everybody.."
      \if broadcast_selected_only
         im\/ "Broadcasting to selected users:" 220_xcoel
         \sloop selected_volunteers
            im\/ moo->full_name
         
      \if broadcast_targetuser
         im\/ "Sending message to " -
         im\/ broadcast_targetuser->full_name 33_xcoel
      im\-------------
      
      im\ pushfont 2
      
         \if has_oauth_token_xnl() && has_provisional_number_xnl()
            \if im\ button "send"
               performBroadcast();
               im\ closePopup
         \else
            im\ button "can't send yet" ColorInfo(0.12f)
            im\ tooltip "we don't have a provisional number yet"
         
         im\ sameLine
         
         \if im\ button "cancel"
            im\ closePopup
      
      
   
//////////////////////////////////////////////////////////////////////////////////////////
Manti/performBroadcast()
   \if has_oauth_token_xnl() && has_provisional_number_xnl()
   \else
      return; // can't do jack shit here
   
   // here here
   vector<string> recipient_list;
   \if broadcast_everybody
      \sloopmap volunteers
         recipient_list.push_back(moo.name);
   \elif broadcast_selected_only
      \sloop selected_volunteers
         recipient_list.push_back(moo->name); // name is the number I guess
   \elif broadcast_targetuser
      recipient_list.push_back(broadcast_targetuser->name); // just that number thanks
   
   
   \sloop recipient_list
      performMessageSending(moo, broadcast_message_buildup);
   
   auto& mo = createNewMessageObject();
   mo.is_outbound = true;
   mo.outbound_numbers = recipient_list;
   mo.message = broadcast_message_buildup; // I guess that's that
   mo.save();
   
   message_synced = false;
   
//////////////////////////////////////////////////////////////////////////////////////////
Manti/rebroadcast(Message& lemessage)
   
   // okay, so.. let's broadcast
   vector<string> recipient_list;
   
   string used_message = lemessage.message;
   if(Morestring::beginsWith(used_message, "REBROADCAST")){
   } else {
      used_message = "REBROADCAST:\n" + lemessage.message;
   }
   
   \sloopmap volunteers
      \if moo.name != lemessage.inbound_number
         recipient_list.push_back(moo.name);
         
   \sloop recipient_list
      performMessageSending(moo, used_message);
   
   auto& mo = createNewMessageObject();
   mo.is_outbound = true;
   mo.outbound_numbers = recipient_list;
   mo.message = used_message; // I guess that's that
   mo.save();
   
   message_synced = false;
   
//////////////////////////////////////////////////////////////////////////////////////////
Manti/performMessageSending(const string& target_number, const string& messagebui)
   
   RawMessage rm;
   rm.outbount = true;
   // no message id
   rm.out_addr = target_number;
   rm.message = messagebui;
   
   // alright
   bool demomode = demo_phone_number.size() != 0;
   \if demomode && target_number != demo_phone_number
      // just a fake one
      rm.not_actually_sent = true;
   \else
      
      ripcurling("https://tapi.telstra.com/v2/messages/sms")
         .json_oauth(oauth_token)
         ("to", target_number)
         //("from", "+61472881281") // source address that'll be displayed on the receiving device
         ("from", provisional_number) // source address that'll be displayed on the receiving device
         ("body", messagebui)
         ("validity", 125) // minutes to keep trying to send the message
         //("scheduledDelivery", 1) // delay in minutes, until the platform should try send the message
         //("notifyURL", "") // url that'll be called once the message is prcessed
         //("replyRequest", true) // if true, reply message functionality will be implemented
            // if false, normal message handling is implemented. if true a temporary number is used to deliver this message
            // if true, should not use a FROM field, I guess
         //("priority", true) // means try send this one before the preceding one
         //("receiptOff", true) // true means delivery receipts will be ignored
         / [](auto& res){
            
            \\/ "Message sent!"
            res.print();
            
         };
         
   raw_messages.push_front(rm);
   
//////////////////////////////////////////////////////////////////////////////////////////
Manti/openNewTaskDialogue()
   \\/ "New task.."
   
   task_description_focus = 30;
   im\ openModal "new task" /moda/
      im\ pushfont 3 /
         im\/ "New Task"
      
      im\----------------------------------
      
      im\/ "you can assign volunteers to tasks."
      im\/ "they'll get notified of the task, and so on."
      
      im\----------------------------------
      im\/ "description"
      im\ multiLineInput "description" task_description 600 -1 5
      im\ maybeFocusKeyboardHere task_description_focus
      
      im\----------------------------------
      im\/ "notes"
      im\ multiLineInput "notes" task_notes 600 -1 5
      
      im\/ "note: task notes is not visible to volunteers"
      im\----------------------------------
      
      \if im\ button "create task"
         createNewTask(task_description, task_notes);
         im\ closePopup
      
      im\ sameLine
         
      \if im\ button "cancel"
         im\ closePopup
      
//////////////////////////////////////////////////////////////////////////////////////////
Manti/openTaskEdit(const string& taskname)
   
   currently_edited_task = &tasks[taskname];
   
   task_description = currently_edited_task->description;
   task_notes = currently_edited_task->notes;
   
   task_description_focus = 30;
   im\ openModal "edittask" /moda/
      
      im\/ "Note: this thing is still under construction" 196_xcoel
      
      \assert currently_edited_task
      
      im\ pushfont 3 /
         im\/ "Edit Task"
      im\----------------------------------
      im\/ "description"
      im\ multiLineInput "description" task_description 600 -1 5
      im\----------------------------------
      im\/ "notes"
      im\ multiLineInput "notes" task_notes 600 -1 5
      im\ maybeFocusKeyboardHere task_description_focus
      
      im\/ "note: task notes is not visible to volunteers"
      im\----------------------------------
      
      im\----------------------------------
      
      \if im\ button "mark finished"
         im\ closePopup
      
      im\ sameLine
         
      \if im\ button "cancel"
         im\ closePopup
   
//////////////////////////////////////////////////////////////////////////////////////////
Manti/openHelp()
   \\/ "Opening help"
   
   im\ openModal "edittask" /moda/
      
      im\ pushfont 3 /
         im\/ "Help"
   
      im\ textWrapped "This software is under construction. Really, tried to do as much as possible in 1.5 days, but only managed to make a demo prototype."
      im\ textWrapped "Having said that, the prototype functions work!"
      im\--------
      im\ "Get in touch with developer:"
      im\ pushfont 2 /
         im\ "ywid062@gmail.com" 196_xcoel
         im\ "Yose Widjaja" 220_xcoel
         
      \if im\ button "close"
         im\ closePopup
   
//////////////////////////////////////////////////////////////////////////////////////////
Manti/openVolunteerPopup(const string& volname)
   // maybe later amigo
   
   currently_popping_vol = &volunteers[volname];
   
   im\ openPopup "volpopup" /moda/
      
      \if im\ button "message"
         // let's see
         // gotta rebroadcast
         want_message_vol = currently_popping_vol;
         im\ closePopup
         
      im\ sameLine
      im\ helpMarker "send a message to this user"
      
      \if im\ button "select"
         // let's see
         // gotta rebroadcast
         // also not done yet
         im\ closePopup
         
      im\ sameLine
      im\ helpMarker "select the item"
      
      \if im\ button "edit"
         // let's see
         // gotta rebroadcast
         // also not done yet
         im\ closePopup
         
      im\ sameLine
      im\ helpMarker "edit the message"
      
      // rebroadcast
      // delete
   
//////////////////////////////////////////////////////////////////////////////////////////
Manti/openMessageContextMenu(const string& lemessageid)
   
   currently_popping_message = &messages[lemessageid];
   
   im\ openPopup "messagepopup" /moda/
      
      
      \if im\ button "rebroadcast"
         // let's see
         // gotta rebroadcast
         \if currently_popping_message
            rebroadcast(currently_popping_message.ref());
         im\ closePopup
         
      im\ sameLine
      im\ helpMarker "this will rebroadcast the message to everybody"
      
      \if im\ button "delete"
         // let's see
         // gotta rebroadcast
         \if currently_popping_message
            deleteMessage(currently_popping_message.ref());
         im\ closePopup
         
      im\ sameLine
      im\ helpMarker "this will hide the message, but yeah"
      
      \if currently_popping_message && currently_popping_message->is_outbound
         currently_popping_message->sync();
         im\-----------------------------
         im\/ "Recipients:"
         \sloop currently_popping_message->broadcast_targets
            im\/ moo->full_name 33_xcoel
            
         \sloop currently_popping_message->unknown_broadcast_targets
            im\/ moo 240_xcoel
      
      
      // rebroadcast
      // delete
   
//////////////////////////////////////////////////////////////////////////////////////////
Manti/deleteMessage(Message& lemessage)
   lemessage.eraseSelf();
   message_synced = false;
   
//////////////////////////////////////////////////////////////////////////////////////////
Manti/messagesPaneImcode()
   im\ pushfont 2 /
      im\/ "Messages"
      
   im\ sameLine
   \if im\ button "broadcast"
      openBroadcastMenu(true, false, NULL);
      
   im\ sameLine
   \if selected_volunteers.size() != 0
      \if im\ button "send"
         openBroadcastMenu(false, true, NULL);
      
   im\ sameLine
   \if im\ button "logs"
      openRawMessagesList();
      
   im\-------------------
   im\ inputText "filter" message_filter 150
   im\-------------------
   
   syncMessages();
   
   
   
   string message_to_popup;
   
   im\ scrollRegion("items", -1, _roen->viewheight - 96) /&/
      
      \sloop sorted_messages, moo, counta
         // well it's an item
         \if counta != 0
            im\------------------
         
         im\ idpush counta /&/
            
            moo->sync();
            // here here
            
            bool was_clicked = false;
            
            \if moo->is_inbound
               im\/ "from" 33_xcoel -
               was_clicked = was_clicked || im\clicked();
               im\/ moo->getInboundLabel()
               was_clicked = was_clicked || im\clicked();
               
            \else
            
               \if moo->outbound_numbers.size() == 1
                  im\/ "message to" 112_xcoel -
                  was_clicked = was_clicked || im\clicked();
                  \if moo->broadcast_targets.size() != 0
                     im\/ moo->broadcast_targets[0]->full_name
                  \elif moo->unknown_broadcast_targets.size() != 0
                     im\/ moo->unknown_broadcast_targets[0]
                  was_clicked = was_clicked || im\clicked();
               \else
            
                  im\/ "broadcast" 112_xcoel -
                  was_clicked = was_clicked || im\clicked();
                  
                  im\/ (__toString(moo->outbound_numbers.size()) + " recipients");
                  was_clicked = was_clicked || im\clicked();
               
            // message body
            im\ textWrapped moo->message
            was_clicked = was_clicked || im\clicked();
            
            im\/ moo->date.secDiffString() 240_xcoel
            was_clicked = was_clicked || im\clicked();
            
            \if was_clicked
               message_to_popup = moo->name; // I guess by ID is alright
               
            
   \if message_to_popup.size() != 0
      openMessageContextMenu(message_to_popup);
   

//////////////////////////////////////////////////////////////////////////////////////////
Manti/tasksPaneImcode()
   im\ pushfont 2 /
      im\/ "Tasks"
   
   im\ sameLine
   \if im\ button "new task"
      openNewTaskDialogue();
   
   im\-------------------
   im\ inputText "filter" tasks_filter 150
   im\-------------------
   syncTasks();
   
   string task_to_edit;
   
   im\ scrollRegion("items", -1, _roen->viewheight - 96) /&/
      
      \sloop sorted_tasks, moo, counta
         // well it's an item
         \if counta != 0
            im\/ ""
         
         im\ idpush counta /&/
         
            moo->sync();
            
            im\ textWrapped moo->description 220_xcoel
            
            im\-----------------
            
            // next line
            im\/ "Last update " 240_xcoel -
            im\/ moo->last_update.secDiffString() 240_xcoel
            
               
            im\ sameLine
            \if im\ button "edit"
               // open menu
               task_to_edit = moo->name;
            
            \if selected_volunteers.size() != 0
               im\ sameLine
               \if im\ button "assign"
                  assignSelectedVolunteersTo(*moo); // I guess
                  
               im\ tooltip "Assign those contacts into this task, notifying them while at it"
            \else
               im\ sameLine
               im\ button "assign" ColorInfo(0.2f)
               im\ tooltip "select contacts first to assign to this task"
                  // hmm, assign users
            

            \if moo->assignments.size() != 0
               im\ sameLine
               \if im\ button "unassign"
                  clearTaskAssignment(*moo);
               im\ tooltip "Unassign from this task"
            
            im\-----------------
            \if moo->assignments.size() == 0
               im\/ "nobody assigned to this task" 240_xcoel
            \else
               \sloop moo->vol_assignees, assignee, counta
                  \if counta >= 2
                     int leftover = moo->vol_assignees.size() - counta;
                     im\/ ("and " + __toString(leftover) + " others assigned");
                     break;
                  
                  im\/ assignee->full_name -
                  im\/ "assigned" 240_xcoel
                  
                  
               // well, go through those eh?
               
            
   \if task_to_edit.size() != 0
      openTaskEdit(task_to_edit);
      
      
//////////////////////////////////////////////////////////////////////////////////////////
Manti/assignSelectedVolunteersTo(Task& letask)
   // sounds good eh
   
   // let's see
   
   \sloop selected_volunteers
      letask.addVolunteer(moo, this);
   
   tasks_synced = false;
   
   \sloop selected_volunteers
      moo->selected = false;
   selected_vols_synced = false;
      
   
   
//////////////////////////////////////////////////////////////////////////////////////////
Manti/clearTaskAssignment(Task& letask)
   
   letask.clearVolunteers();
   // let's see
   
//////////////////////////////////////////////////////////////////////////////////////////
Manti/volunteersPaneImcode()
   im\ pushfont 2 /
      im\/ "contacts"
      
   im\ sameLine
   \if im\ button "import contacts"
      openContactsImportMenu();
      
   im\-------------------
   im\ inputText "filter" volunteer_filter 150
   im\-------------------
   syncContacts();
   syncContactSel();
   
   string contact_to_popup;
   
   \if want_message_vol
      openBroadcastMenu(false, false, want_message_vol);
      want_message_vol = NULL;
   
   
   im\ scrollRegion("items", -1, _roen->viewheight - 96) /&/
      
      \sloop sorted_volunteers, moo, counta
         // well it's an item
         \if counta != 0
            im\------------
         
         im\ idpush counta /&/
            
            
            moo->sync();
            
            \if !moo->selected
               im\/ "SEL" 240_xcoel -
               im\ tooltip "click to select this contact"
            \else
               im\/ "SEL" 112_xcoel -
               im\ tooltip "click to unselect this contact"
               
            \if im\ clicked
               moo->selected = !moo->selected;
               selected_vols_synced = false;
            
            
            bool was_clicked = false;
            
            \if moo->selected
               im\/ moo->full_name 33_xcoel
            \else
               im\/ moo->full_name
            
            was_clicked = was_clicked || im\clicked();
            
            \if im\isItemHovered
               im\ tooltip moo->name
                  // the phone number
               
            im\ sameLine
            im\/ moo->notes 240_xcoel
            
            was_clicked = was_clicked || im\clicked();
            
               
            \if moo->taskptrs.size() != 0
               im\/ ("assigned to " + __toString(moo->taskptrs.size()) + " tasks", 244_xcoel);
               was_clicked = was_clicked || im\clicked();
               im\ tooltip([&](auto& imo){
                  \sloop moo->taskptrs, letask
                     im\ text(Morestring::firstN(letask->description, 40, ".."));
               });
               
            \if was_clicked
               contact_to_popup = moo->name;
               // I guess so yeah
            
            
            // task assignments
               
            
   \if contact_to_popup.size() != 0
      openVolunteerPopup(contact_to_popup);
   
      
//////////////////////////////////////////////////////////////////////////////////////////
Manti/setupImlets()
   
   
   int viewwidth = _roen->viewwidth;
   
   int panel_width = (viewwidth / 3) - 8; // 4 pixels on each side
   
   
   imlet("titler").noBackground().noScroll().fixedPane().posiRTL(4,4).width(200).height(44) /imo/
      \if command_hq_name.val().size() == 0
         im\/ "SHADOW" 33_xcoel -
         im\/ "MANTISSA" 112_xcoel -
      \else
         im\/ command_hq_name.val() 220_xcoel -
      im\/ " powered by TelstraDev SMS API"
      
      
      \if has_oauth_error_xnl()
         \if im\ button "error detected, click here"
            openSettings();
      \elif !has_oauth_token_xnl()
         \if client_key->size() == 0 || client_secret->size() == 0
            \if im\ button "setup client key/secret first!"
               openSettings();
         \else
            im\ pushfont 2 /
               im\/ "obtaining oauth token.." -
               im\ asciiSpinner
               
            im\text ("(attempt " + __toString(auth_token_tries_count) + ")", 33_xcoel);
      \else
         // has auth token, but do we have phone number?
         \if !has_provisional_number_xnl()
            im\ pushfont 2 /
               im\/ "getting provi. number.." -
               im\ asciiSpinner
            
            im\text ("(attempt " + __toString(phone_number_tries_count) + ")", 33_xcoel);
         \else
            im\ pushfont 3/
               im\/ provisional_number 33_xcoel
            
            \if last_succ_messsage_check.isNull()
               im\/ "no messages check done yet"
            \elif last_succ_messsage_check.hasPassed(30)
               im\/ "last message check : " 196_xcoel -
               im\/ last_succ_messsage_check.secDiffString() -
               im\/ " (too long ago)"
            \elif !last_message_receive.isNull()
               im\/ "last message : " -
               im\/ last_message_receive.secDiffString() 33_xcoel
            \else
               im\/ "no messages received yet"
            
            
               
         
   imlet("menuer").fixedPane().posiRTR(4,6).width(viewwidth - 200 - 8).height(26) /imo/
      im\ pushfont 2.0f /
      
         im\ sameLine
         im\AlignTextToFramePadding
         \if im\ button "BROADCAST"
            openBroadcastMenu(true, false, NULL);
            
         im\ sameLine
         im\AlignTextToFramePadding
         im\/ " | " 240_xcoel
            
         im\ sameLine
         im\AlignTextToFramePadding
         \if im\ button "IMPORT"
            openContactsImportMenu();
            
         im\ sameLine
         im\AlignTextToFramePadding
         im\/ " | " 240_xcoel
         
         im\ sameLine
         im\AlignTextToFramePadding
         \if im\ button "SETTINGS"
            openSettings();
            
         im\ sameLine
         im\AlignTextToFramePadding
         im\/ " | " 240_xcoel
         
         im\ sameLine
         im\AlignTextToFramePadding
         \if im\ button "SMS LOGS"
            openRawMessagesList();
            
         im\ sameLine
         im\AlignTextToFramePadding
         im\/ " | " 240_xcoel
            
         im\ sameLine
         im\AlignTextToFramePadding
         \if im\ button "HELP"
            openHelp();
         
   
   imlet("messagespane").fixedPane().posiRTL(4,52).width(panel_width).heightRoenMinus(56) /imo/
      messagesPaneImcode();
      
      
   imlet("tasklist").fixedPane().noBG().posiRTM(52).width(panel_width).heightRoenMinus(56) /imo/
      tasksPaneImcode();
      
   imlet("volunteerlist").fixedPane().posiRTR(4,52).width(panel_width).heightRoenMinus(56) /imo/
      volunteersPaneImcode();
   
   
//////////////////////////////////////////////////////////////////////////////////////////
Manti/setupKeys()
   
   
   
   // we should be using keygate, but meh
   _keyceptor[0](rk_r)("R!") / [this](auto rkd){
      \\/ "R PRESSED - RESTARTING!"
      
      return true;
      
   };
   
//////////////////////////////////////////////////////////////////////////////////////////
Manti/update()

   \if __roenupdp % 60 == 0
      
      \if has_oauth_token_xnl() && oauth_token_expiry.hasPassed()
         has_oauth_token_xnl = false;
         // time to expire it
      
      // every 1.5 secs, for now
      \if !has_oauth_error_xnl() && !has_oauth_token_xnl() && client_key->size() != 0 && client_secret->size() != 0
         \if auth_token_tries_count < 10
            auth_token_tries_count++;
            ripcurling("https://tapi.telstra.com/v2/oauth/token")
               .urlencoded()
               ("client_id", client_key.val())
               ("client_secret", client_secret.val())
               ("grant_type", "client_credentials")
               ("scope", "NSMS") /res/
                  
                  \if res
                     has_oauth_error_xnl = false;
                     has_oauth_token_xnl = true;
                     oauth_token = res["access_token"];
                     int expirysecs = Morestring::toInt(res["expires_in"]);
                     oauth_token_expiry = SDate() + (expirysecs - 120);
                  \else
                     has_oauth_error_xnl = true;
                     last_oauth_error = res.contents;
                     oauth_error_date = SDate();
         
      \if has_oauth_token_xnl() && !has_provisional_number_xnl()
         \if phone_number_tries_count < 10
            phone_number_tries_count++;
            
            ripcurling("https://tapi.telstra.com/v2/messages/provisioning/subscriptions")
               .json_oauth(oauth_token)
               ("activeDays", 30) /res/
                  
                  \if !res
                     // something failed.. but we don't have a console yet
                  \else
                     string destaddress = res["destinationAddress"];
                     \if destaddress.size() != 0
                        has_provisional_number_xnl = true;
                        provisional_number = destaddress;
                        // screw expiry date for now
   
      \if has_provisional_number_xnl() && has_oauth_token_xnl()
         // and has oauth as well, I guess
         // try get messages
         ripcurling("https://tapi.telstra.com/v2/messages/sms")
            .getmode()
            .json_oauth(oauth_token) /res/
               
               // let's check it
               \if res
                  \if res("message")
                     string messageid = res["messageId"];
                     string message = res["message"];
                     string sender = res["senderAddress"];
                     receiveMessageRaw(messageid, message, sender);
                     last_message_receive = SDate();
                     // here here
                  
                  last_succ_messsage_check = SDate();
                     
   
//////////////////////////////////////////////////////////////////////////////////////////
Manti/receiveMessageRaw(const string& theid, const string& themessage, const string& sender)
   
   RawMessage rm;
   rm.inbound = true;
   rm.message_id = theid;
   rm.out_addr = sender;
   rm.message = themessage;
   // not processed, but yeah
   raw_messages.push_front(rm); // start at the front, I guess
   
   onReceiveMessage(rm);
   
//////////////////////////////////////////////////////////////////////////////////////////
Manti/createNewMessageObject() -> Message&
   SDate rightnow;
   static int message_counter = 0;
   message_counter++;
   string theid = __toString(rightnow.getUnixSeconds()) + ":" + __toString(message_counter);
   return messages[theid];
//////////////////////////////////////////////////////////////////////////////////////////
Manti/onReceiveMessage(RawMessage& rm)

   //. well, 
   auto& lemess = createNewMessageObject();
   lemess.is_inbound = true;
   lemess.inbound_number = rm.out_addr;
   lemess.message = rm.message;
   lemess.date = rm.received_time; //. whatever
   lemess.save();
   
   message_synced = false;
   
//////////////////////////////////////////////////////////////////////////////////////////
Manti/drawPost2D()

   // what here? nothing yet..
            
   
//////////////////////////////////////////////////////////////////////////////////////////
Manti/handleTouchEvent(const TouchInfo& tinfoil) -> bool
   return false;
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
Manti/createNewTask(const string& thedesc, const string& thenotes) -> Task&
   string taskid = __toString(SDate().getUnixSeconds()); // so hacky, lel
   auto& ret = tasks[taskid];
   
   ret.description = thedesc;
   ret.notes = thenotes;
   tasks_synced = false;
   
   ret.save();
   return ret;
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

