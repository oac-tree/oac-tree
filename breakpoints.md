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

