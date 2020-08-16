////////////////////////////////////////////////////////////
#pragma once
namespace std{}; using namespace std;
////////////////////////////////////////////////////////////
#include "cloth_classdef.cl.hpp"
#include "../lmdb/lmap.hpp"
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
namespace Mantissa{
class SearchableBase{
   public:
   //-------------------------------------------------------
   bool synced = false;
   bool selected = false;
   vOLUNTEER inbound_vol;
   vector<tAG> searchable_tags;
   vector<vOLUNTEER> broadcast_targets;
   vector<string> unknown_broadcast_targets;
   string estimated_firstname;
   //-------------------------------------------------------
};
////////////////////////////////////////////////////////////
class SurveyAnswerBase{
   public:
   //-------------------------------------------------------
   vOLUNTEER source_volunteer;
   bool synced = false;
   //-------------------------------------------------------
};
////////////////////////////////////////////////////////////
class SurveyAnswer:public SurveyAnswerBase{
   public:
   //-------------------------------------------------------
   string source_number;
   string answer;
   //-------------------------------------------------------
   flector_subbie_decl;
   SurveyAnswer();
   void initflec();
   void sync();
   //-------------------------------------------------------
};
////////////////////////////////////////////////////////////
class Message:public lmapTHING, public SearchableBase, public Softpointable{
   public:
   //-------------------------------------------------------
   string name;
   bool is_inbound = false;
   bool is_outbound = false;
   string message;
   bool is_survey = false;
   vector<string> survey_choices;
   vector<string> outbound_numbers;
   string inbound_number;
   SDate date;
   //-------------------------------------------------------
   flector_subbie_decl;
   Message();
   void initflec();
   void sync();
   string getInboundLabel();
   //-------------------------------------------------------
};
////////////////////////////////////////////////////////////
class VolunteerBase{
   public:
   //-------------------------------------------------------
   vector<tASK> taskptrs;
   //-------------------------------------------------------
};
////////////////////////////////////////////////////////////
class Volunteer:public lmapTHING, public SearchableBase, public Softpointable, public VolunteerBase{
   public:
   //-------------------------------------------------------
   string name;
   string full_name;
   string firstname;
   vector<string> tags;
   string notes;
   vector<string> assigned_tasks;
   SDate date;
   //-------------------------------------------------------
   flector_subbie_decl;
   Volunteer();
   void initflec();
   void sync();
   void addTask(const string& thetask_id);
   void removeTask(const string& thetask_id);
   //-------------------------------------------------------
};
////////////////////////////////////////////////////////////
class TaskAssignment{
   public:
   //-------------------------------------------------------
   string volunid;
   char task_letter = 0;
   bool allow_accept = false;
   bool allow_refuse = false;
   bool allow_done = false;
   bool accepted = false;
   bool refused = false;
   bool marked_done = false;
   bool message_sent = false;
   //-------------------------------------------------------
   flector_subbie_decl;
   TaskAssignment();
   void initflec();
};
////////////////////////////////////////////////////////////
class TaskUpdate{
   public:
   //-------------------------------------------------------
   string text;
   SDate time;
   //-------------------------------------------------------
   flector_subbie_decl;
   TaskUpdate();
   void initflec();
};
////////////////////////////////////////////////////////////
class TaskBase{
   public:
   //-------------------------------------------------------
   vector<vOLUNTEER> vol_assignees;
   //-------------------------------------------------------
};
////////////////////////////////////////////////////////////
class Task:public lmapTHING, public SearchableBase, public Softpointable, public TaskBase{
   public:
   //-------------------------------------------------------
   string name;
   string description;
   string notes;
   SDate creation_date;
   vector<TaskUpdate> updates;
   SDate last_update;
   vector<TaskAssignment> assignments;
   bool done_signalled = false;
   bool completed = false;
   SDate completion_date;
   //-------------------------------------------------------
   flector_subbie_decl;
   Task();
   void initflec();
   void sync();
   void addVolunteer(Volunteer * leteer, Manti * lemanti);
   void clearVolunteers();
   //-------------------------------------------------------
};
////////////////////////////////////////////////////////////
class TagBase{
   public:
   //-------------------------------------------------------
   bool selected = false;
   //-------------------------------------------------------
};
////////////////////////////////////////////////////////////
class Tag:public lmapTHING, public Softpointable, public TagBase{
   public:
   //-------------------------------------------------------
   string name;
   int assigned_id = 0;
   //-------------------------------------------------------
   flector_subbie_decl;
   Tag();
   void initflec();
};
////////////////////////////////////////////////////////////
class RawMessage{
   public:
   //-------------------------------------------------------
   bool inbound = false;
   bool outbount = false;
   bool not_actually_sent = false;
   string message_id;
   string out_addr;
   string message;
   SDate received_time;
   bool processed = false;
   //-------------------------------------------------------
   flector_subbie_decl;
   RawMessage();
   void initflec();
};
////////////////////////////////////////////////////////////
class SearchPredPart{
   public:
   //-------------------------------------------------------
   string text;
   vector<Tag*> tags;
   bool got_any = false;
   //-------------------------------------------------------
   bool matchCheck(const string& thestr);
   //-------------------------------------------------------
};
////////////////////////////////////////////////////////////
class SearchPred{
   public:
   //-------------------------------------------------------
   vector<SearchPredPart> parts;
   int matchcount = 0;
   //-------------------------------------------------------
   bool finishMatch(const string& thestr);
   void clearBools();
   bool matches(Volunteer& levol);
   bool matches(Task& task);
   bool matches(Message& mess);
   //-------------------------------------------------------
};
////////////////////////////////////////////////////////////
vOLUNTEER findVolunteer(const string& phonenum);
////////////////////////////////////////////////////////////
DECLlmdbmap(messages, Message, "mantissadb");
DECLlmdbmap(volunteers, Volunteer, "mantissadb");
DECLlmdbmap(tasks, Task, "mantissadb");
DECLlmdbmap(tags, Tag, "mantissadb");
}; // end namespace Mantissa
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
