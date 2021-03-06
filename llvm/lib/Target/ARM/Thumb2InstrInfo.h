//===-- Thumb2InstrInfo.h - Thumb-2 Instruction Information -----*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the Thumb-2 implementation of the TargetInstrInfo class.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_ARM_THUMB2INSTRINFO_H
#define LLVM_LIB_TARGET_ARM_THUMB2INSTRINFO_H

#include "ARMBaseInstrInfo.h"
#include "ThumbRegisterInfo.h"

namespace llvm {
class ARMSubtarget;
class ScheduleHazardRecognizer;

class Thumb2InstrInfo : public ARMBaseInstrInfo {
  ThumbRegisterInfo RI;
public:
  explicit Thumb2InstrInfo(const ARMSubtarget &STI);

  /// Return the noop instruction to use for a noop.
  void getNoop(MCInst &NopInst) const override;

  // Return the non-pre/post incrementing version of 'Opc'. Return 0
  // if there is not such an opcode.
  unsigned getUnindexedOpcode(unsigned Opc) const override;

  void ReplaceTailWithBranchTo(MachineBasicBlock::iterator Tail,
                               MachineBasicBlock *NewDest) const override;

  bool isLegalToSplitMBBAt(MachineBasicBlock &MBB,
                           MachineBasicBlock::iterator MBBI) const override;

  void copyPhysReg(MachineBasicBlock &MBB, MachineBasicBlock::iterator I,
                   const DebugLoc &DL, MCRegister DestReg, MCRegister SrcReg,
                   bool KillSrc) const override;

  void storeRegToStackSlot(MachineBasicBlock &MBB,
                           MachineBasicBlock::iterator MBBI,
                           Register SrcReg, bool isKill, int FrameIndex,
                           const TargetRegisterClass *RC,
                           const TargetRegisterInfo *TRI) const override;

  void loadRegFromStackSlot(MachineBasicBlock &MBB,
                            MachineBasicBlock::iterator MBBI,
                            Register DestReg, int FrameIndex,
                            const TargetRegisterClass *RC,
                            const TargetRegisterInfo *TRI) const override;

  /// getRegisterInfo - TargetInstrInfo is a superset of MRegister info.  As
  /// such, whenever a client has an instance of instruction info, it should
  /// always be able to get register info as well (through this method).
  ///
  const ThumbRegisterInfo &getRegisterInfo() const override { return RI; }

private:
  void expandLoadStackGuard(MachineBasicBlock::iterator MI) const override;
};

/// getITInstrPredicate - Valid only in Thumb2 mode. This function is identical
/// to llvm::getInstrPredicate except it returns AL for conditional branch
/// instructions which are "predicated", but are not in IT blocks.
ARMCC::CondCodes getITInstrPredicate(const MachineInstr &MI, unsigned &PredReg);

// getVPTInstrPredicate: VPT analogue of that, plus a helper function
// corresponding to MachineInstr::findFirstPredOperandIdx.
int findFirstVPTPredOperandIdx(const MachineInstr &MI);
ARMVCC::VPTCodes getVPTInstrPredicate(const MachineInstr &MI,
                                      unsigned &PredReg);
inline ARMVCC::VPTCodes getVPTInstrPredicate(const MachineInstr &MI) {
  unsigned PredReg;
  return getVPTInstrPredicate(MI, PredReg);
}
} // namespace llvm

#endif
