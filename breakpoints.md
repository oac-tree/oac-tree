# Breakpoints

## Runner

```c++
Runner::SetBreakpoint(Instruction* instr)
{
  // If not present yet:
  m_breakpoints.push_back(instr);
}

bool Runner::BreakpointReached()
{
  if (!m_breakpoints.empty())
  {
    Tree<Instruction*> next_instructions = m_proc->NextInstructions();
    // HandleIntersection only takes into account active breakpoints:
    // - Stop on first encounter of breakpoint and release the breakpoint;
    // - Do not stop on released breakpoints, but make them active again.
    auto status = CheckBreakpoints(m_breakpoints, next_instructions);
    if (status == Breakpoint::Break)
    {
      return true;
    }
  }
  return false;
}

void Runner::ExecuteProcedure()
{
  m_halt.store(false);
  if (m_proc)
  {
    auto sleep_time_ms = TickTimeoutMs(*m_proc);

    while (!IsFinished() && !m_halt.load())
    {
      if (BreakpointReached())
      {
        return;
      }
      ExecuteSingle();
      if (IsRunning())
      {
        std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time_ms));
      }
    }
  }
}

```

## Parallel sequence logic and statuses

To be able to query the list of instructions that will be executed next (from the root instruction), the implementation of the ParallelSequence had to be adapted. This has no impact on any unit test or other instructions.

With this change, it is now possible to know exactly which instructions will be executed next if `Procedure::ExecuteSingle` will be called, thus allowing to handle breakpoints at the top level, i.e. the `Runner` class.

This change also prioritizes the `NOT_FINISHED` execution status above the `RUNNING` status, thus removing the necessity to sleep for a short while before ticking the root instruction again.

Old implementation:

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

AsyncWrapper::UpdateStatus()
{
  // If child thread exists and is finished:
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
