# Breakpoints

## Instruction

```c++
void Instruction::ExecuteSingle(UserInterface& ui, Workspace& ws)
{
  if (m_breakpoint_state == Breakpoint::Set)
  {
    m_breakpoint_callback();
    return;
  }
  else if (m_breakpoint_state == Breakpoint::Released)
  {
    m_breakpoint_state == Breakpoint::Set;
  }
  Preamble(ui);
  m_status_before = GetStatus();
  SetStatus(ExecuteSingleImpl(ui, ws));
  Postamble(ui);
}
```

## Runner

```c++
Runner::SetBreakpoint(Instruction* instr)
{
  if (instr->HasBreakpoint())
  {
    return;
  }
  auto callback = [this, instr](){
    BreakPointReached(instr);
  };
  instr->SetBreakpoint(callback);
}

Runner::BreakPointReached(Instruction* instr)
{
  m_active_breakpoints.push(instr);  // threadsafe FIFO
}

void Runner::ExecuteProcedure()
{
  m_halt.store(false);
  if (m_current_breakpoint)
  {
    m_current_breakpoint.release();
    m_current_breakpoint.clear();
  }
  if (m_proc)
  {
    auto sleep_time_ms = TickTimeoutMs(*m_proc);

    while (!IsFinished() && !m_halt.load())
    {
      ExecuteSingle();
      if (!m_active_breakpoints.empty())
      {
        m_current_breakpoint = m_active_breakpoints.pop();
        return;
      }
      if (IsRunning())
      {
        std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time_ms));
      }
    }
  }
}

```

## Parallel sequence logic and statuses

Current implementation:

```c++
ParallelSequence::ExecuteSingleImpl()
{
  // foreach wrapper:
  {
    wrapper.GetStatus();
    // if NOT success/failure:
    wrapper.Tick();
  }
  // If success/failure threshold is reached, that will be the status
  // Otherwise status is RUNNING
  // Only the wrapper statuses are read
  status = CalculateCompoundStatus();
  if (status != RUNNING)
  {
    Halt();
  }
  return status;
}

AsyncWrapper::GetStatus() const
{
  return m_status;
}

AsyncWrapper::Tick()
{
  // If child thread not finished, return
  // This includes getting the shared (void) state into the future
  // If the child is still running, this means nothing needs to be done
  status = child->GetStatus(); // Ok, because not running
  if (status == SUCCESS/FAILURE)
  {
    m_status = status;
    return;
  }
  LaunchChild();
  m_status = RUNNING;
}
```

New implementation:

```c++
ParallelSequence::ExecuteSingleImpl()
{
  // foreach wrapper:
  {
    // GetStatus just reads the wrapper's status field
    status = wrapper.GetStatus();
    if (status == NOT_STARTED/NOT_FINISHED)
    {
      wrapper.Tick();
    }
    // FetchStatus sets its member m_status to its child status, or leaves it RUNNING
    // if the thread is not finished
    wrapper.FetchStatus();
  }
  // If success/failure threshold is reached, that will be the status
  // Otherwise status is RUNNING
  // Only the wrapper statuses are read
  status = CalculateCompoundStatus();
  if (status == SUCCESS/FAILURE)
  {
    Halt();
  }
  return status;
}

AsyncWrapper::FetchStatus()
{
  // If child thread finished:
  {
    m_status = child->GetStatus();
  }
}

AsyncWrapper::GetStatus() const
{
  return m_status;
}

AsyncWrapper::Tick()
{
  LaunchChild();
  m_status = RUNNING;
}
```
