// Created by Petr Karnakov on 25.04.2018
// Copyright 2018 ETH Zurich

#include <cassert>
#include <iomanip>
#include <sstream>
#include <stdexcept>

#include "suspender.h"

Suspender::Sem::Sem(Suspender& p, std::string name) : p(p), name_(name) {
  auto& l = p.lu_;
  auto& i = p.lui_;

  if (i == l.begin()) {
    // Allow nested calls on first level
    p.nest_ = true;
    // Clear curname_
    p.curname_ = "";
    p.depth_ = 0;
  }

  if (!p.nest_) {
    throw std::runtime_error(
        p.GetCurName() +
        ": Nested calls not allowed. Use Nested() on upper level");
  }
  p.nest_ = false;

  if (std::next(i) == l.end()) {
    l.emplace_back(0, 0);
  }
  ++i;

  i->c = 0;
}

Suspender::Sem::~Sem() {
  auto& l = p.lu_;
  auto& i = p.lui_;

  assert(!l.empty());
  assert(i != l.end());
  assert(i != l.begin());

  auto ip = std::prev(i);

  if (std::next(i) == l.end()) {
    // all lower levels done, next stage
    ++i->t;
    // i->c keeps total number of stages
    if (i->c == i->t || i->c == 0) {
      // all stages done or no stages, remove current level
      l.pop_back();
    }
  }
  i = ip;
}

bool Suspender::Sem::Next(std::string suff) {
  auto& i = p.lui_;
  if (i->c++ == i->t) {
    if (p.curname_ != "") {
      p.curname_ += " --> ";
    }
    std::stringstream st;
    st << std::setfill('0') << std::setw(2) << i->t;
    p.curname_ += name_ + ":" + st.str() + ":" + suff;
    ++p.depth_;
    return true;
  }
  return false;
}

bool Suspender::Sem::operator()(std::string suff) {
  p.nest_ = false;
  return Next(suff);
}

bool Suspender::Sem::Nested(std::string suff) {
  p.nest_ = true;
  return Next(suff);
}

// The index of each stage is given by c.
// LoopBegin and LoopEnd are also stages.
// lb -- index of LoopBegin
// le -- index of LoopEnd

void Suspender::Sem::LoopBegin() {
  U& u = *p.lui_;
  if (u.c == u.t) {
    if (u.lb < u.c) {
      u.lb = u.c;
    }
    ++u.t;
  }
  ++u.c;
}

void Suspender::Sem::LoopBreak() {
  U& u = *p.lui_;
  u.t = u.le; // set target beyond loop end
}

// Important to initialize le even before t reaches LoopEnd
// to be able to break on first iteration
void Suspender::Sem::LoopEnd() {
  U& u = *p.lui_;
  if (u.le < u.lb && // le not initialized for current loop
      u.lb <= u.t) { // target is within the loop
    u.le = u.c;
  }
  if (u.c == u.t + 1) { // passed loop end
    u.t = u.lb; // reset target to loop begin
  }
  ++u.c;
}

Suspender::Suspender() : nest_(false) {
  lu_.emplace_back(-1, -1);
  lui_ = lu_.begin();
}

Suspender::Sem Suspender::GetSem(std::string name) {
  return Sem(*this, name);
}

std::string Suspender::GetCurName() const {
  return curname_;
}

std::string Suspender::Print() const {
  std::stringstream b;
  for (auto& e : lu_) {
    b << "(" << e.c << " " << e.t << ") ";
  }
  return b.str();
}

bool Suspender::Pending() const {
  return lu_.size() != 1;
}
