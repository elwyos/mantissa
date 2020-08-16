////////////////////////////////////////////////////////////
#include "items.cl.hpp"
#include "mantissa.cl.hpp"
#include "items.ceeferinc.cl.hpp"
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
namespace Mantissa{
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
Message::Message(){
   initflec();
}
////////////////////////////////////////////////////////////
void Message::initflec(){
   flector_defstart(2)
      flector_field(name).fromVersion(2);
      flector_field(is_inbound).fromVersion(2);
      flector_field(is_outbound).fromVersion(2);
      flector_field(message).fromVersion(2);
      flector_field(is_survey).fromVersion(2);
      flector_vecfield(survey_choices).fromVersion(2);
      flector_vecfield(outbound_numbers).fromVersion(2);
      flector_field(inbound_number).fromVersion(2);
      flector_field(date).fromVersion(2);
   flector_defend
}
////////////////////////////////////////////////////////////
void Message::sync(){
   if(synced){
      return;
   }
   inbound_vol = findVolunteer(inbound_number);
   broadcast_targets.clear();
   unknown_broadcast_targets.clear();
   sloopvec(outbound_numbers, moo)
      auto vollo = findVolunteer(moo);
      if(vollo){
         broadcast_targets.push_back(vollo);
      }
      else {
         unknown_broadcast_targets.push_back(moo);
      }
   sloopend
   synced = true;
}
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
string Message::getInboundLabel(){
   if(inbound_vol){
      return inbound_vol->full_name;
   }
   return inbound_number;
}
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
RawMessage::RawMessage(){
   initflec();
}
////////////////////////////////////////////////////////////
void RawMessage::initflec(){
   flector_defstart(3)
      flector_field(inbound).fromVersion(2);
      flector_field(outbount).fromVersion(2);
      flector_field(not_actually_sent).fromVersion(3);
      flector_field(message_id).fromVersion(2);
      flector_field(out_addr).fromVersion(2);
      flector_field(message).fromVersion(2);
      flector_field(received_time).fromVersion(2);
      flector_field(processed).fromVersion(2);
   flector_defend
}
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
bool SearchPred::finishMatch(const string& thestr){
   sloopvec(parts, moo)
      if(moo.matchCheck(thestr)){
         matchcount++;
         if(matchcount == parts.size()){
            return true;
         }
      }
   sloopend
   return false;
}
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void SearchPred::clearBools(){
   sloopvec(parts, moo)
      moo.got_any = false;
   sloopend
   matchcount = 0;
}
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
bool SearchPred::matches(Volunteer& levol){
   clearBools();
   if(parts.size() == 0){
      return true;
   }
   if(finishMatch(levol.name)){
      return true;
   }
   if(finishMatch(levol.full_name)){
      return true;
   }
   sloopvec(levol.tags, moo)
      if(finishMatch(moo)){
         return true;
      }
   sloopend
   if(finishMatch(levol.notes)){
      return true;
   }
   return false;
}
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
bool SearchPred::matches(Task& task){
   clearBools();
   if(parts.size() == 0){
      return true;
   }
   if(finishMatch(task.description)){
      return true;
   }
   if(finishMatch(task.notes)){
      return true;
   }
   if(finishMatch(task.notes)){
      return true;
   }
   return false;
}
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
bool SearchPred::matches(Message& mess){
   clearBools();
   if(parts.size() == 0){
      return true;
   }
   if(finishMatch(mess.message)){
      return true;
   }
   return false;
}
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
bool SearchPredPart::matchCheck(const string& thestr){
   if(got_any){
      return false;
   }
   if(Morestring::contains(thestr, text)){
      got_any = true;
      return true;
   }
   return false;
}
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
SurveyAnswer::SurveyAnswer(){
   initflec();
}
////////////////////////////////////////////////////////////
void SurveyAnswer::initflec(){
   flector_defstart(2)
      flector_field(source_number).fromVersion(2);
      flector_field(answer).fromVersion(2);
   flector_defend
}
////////////////////////////////////////////////////////////
void SurveyAnswer::sync(){
   if(synced){
      return;
   }
   synced = true;
}
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
Tag::Tag(){
   initflec();
}
////////////////////////////////////////////////////////////
void Tag::initflec(){
   flector_defstart(2)
      flector_field(name).fromVersion(2);
      flector_field(assigned_id).fromVersion(2);
   flector_defend
}
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
Task::Task(){
   initflec();
}
////////////////////////////////////////////////////////////
void Task::initflec(){
   flector_defstart(2)
      flector_field(name).fromVersion(2);
      flector_field(description).fromVersion(2);
      flector_field(notes).fromVersion(2);
      flector_field(creation_date).fromVersion(2);
      flector_vecfield(updates).fromVersion(2);
      flector_field(last_update).fromVersion(2);
      flector_vecfield(assignments).fromVersion(2);
      flector_field(done_signalled).fromVersion(2);
      flector_field(completed).fromVersion(2);
      flector_field(completion_date).fromVersion(2);
   flector_defend
}
////////////////////////////////////////////////////////////
void Task::sync(){
   if(synced){
      return;
   }
   croust<<"Syncing : "<<(name )<<" / "<<(description )<<" : "<<(assignments.size() )<<" @ "<<((uint64_t) this) << ozendl;
   vol_assignees.clear();
   sloopvec(assignments, moo)
      auto * lesignee = findVolunteer(moo.volunid).rec();
      if(lesignee){
         vol_assignees.push_back(lesignee);
      }
   sloopend
   synced = true;
}
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void Task::addVolunteer(Volunteer * leteer, Manti * lemanti){
   leteer->addTask(name);
   sloopvec(assignments, moo)
      if(moo.volunid == leteer->name){
         return;
      }
   sloopend
   synced = false;
   assignments.push_back(TaskAssignment());
   auto& backa = assignments.back();
   backa.volunid = leteer->name;
   string message;
   message = "You have been assigned a task\n" + description;
   message.append("\nreply OK to acknowledge, NO to refuse");
   message.append("\nreply DONE to signal task is done");
   lemanti->performMessageSending(leteer->name, message);
   save(); // I guess. fuck it
}
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void Task::clearVolunteers(){
   if(assignments.size() == 0){
      return;
   }
   sloopvec(assignments, moo)
      auto * levol = findVolunteer(moo.volunid).rec();
      if(levol){
         levol->removeTask(name);
      }
   sloopend
   assignments.clear();
   save();
   synced = false;
}
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
TaskAssignment::TaskAssignment(){
   initflec();
}
////////////////////////////////////////////////////////////
void TaskAssignment::initflec(){
   flector_defstart(2)
      flector_field(volunid).fromVersion(2);
      flector_field(task_letter).fromVersion(2);
      flector_field(allow_accept).fromVersion(2);
      flector_field(allow_refuse).fromVersion(2);
      flector_field(allow_done).fromVersion(2);
      flector_field(accepted).fromVersion(2);
      flector_field(refused).fromVersion(2);
      flector_field(marked_done).fromVersion(2);
      flector_field(message_sent).fromVersion(2);
   flector_defend
}
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
TaskUpdate::TaskUpdate(){
   initflec();
}
////////////////////////////////////////////////////////////
void TaskUpdate::initflec(){
   flector_defstart(2)
      flector_field(text).fromVersion(2);
      flector_field(time).fromVersion(2);
   flector_defend
}
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
Volunteer::Volunteer(){
   initflec();
}
////////////////////////////////////////////////////////////
void Volunteer::initflec(){
   flector_defstart(3)
      flector_field(name).fromVersion(2);
      flector_field(full_name).fromVersion(2);
      flector_field(firstname).fromVersion(2);
      flector_vecfield(tags).fromVersion(2);
      flector_field(notes).fromVersion(2);
      flector_vecfield(assigned_tasks).fromVersion(3);
      flector_field(date).fromVersion(2);
   flector_defend
}
////////////////////////////////////////////////////////////
void Volunteer::sync(){
   if(synced){
      return;
   }
   taskptrs.clear();
   sloopvec(assigned_tasks, moo)
      auto * letask = tasks(moo);
      if(letask){
         taskptrs.push_back(letask); // yeah
      }
   sloopend
   synced = true;
}
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void Volunteer::addTask(const string& thetask_id){
   sloopvec(assigned_tasks, moo)
      if(moo == thetask_id){
         return; // no point
      }
   sloopend
   assigned_tasks.push_back(thetask_id);
   synced = false;
   save();
}
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void Volunteer::removeTask(const string& thetask_id){
   sloopveccounta(assigned_tasks, moo, counta)
      if(moo == thetask_id){
         assigned_tasks.erase(assigned_tasks.begin() + counta);
         synced = false;
         save();
         return; // no point
      }
   sloopend
}
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
vOLUNTEER findVolunteer(const string& phonenum){
   auto * ret = volunteers(phonenum);
   return ret;
}
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
}; // end namespace Mantissa
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
template class Shing::Softpointer<Mantissa::Message>;
template class Shing::Softpointer<Mantissa::Tag>;
template class Shing::Softpointer<Mantissa::Task>;
template class Shing::Softpointer<Mantissa::Volunteer>;
////////////////////////////////////////////////////////////
