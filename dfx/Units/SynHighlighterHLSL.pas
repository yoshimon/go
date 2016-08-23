{-------------------------------------------------------------------------------
The contents of this file are subject to the Mozilla Public License
Version 1.1 (the "License"); you may not use this file except in compliance
with the License. You may obtain a copy of the License at
http://www.mozilla.org/MPL/

Software distributed under the License is distributed on an "AS IS" basis,
WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License for
the specific language governing rights and limitations under the License.

Code template generated with SynGen.
The original code is: D:\Projekte\VS2013\go_tfs\tools\DirectXFXStudio\Units\SynHighlighterHLSL.pas, released 2014-08-13.
Description: Syntax Parser/Highlighter
The initial author of this file is Gokhan Ozdogan.
Copyright (c) 2014, all rights reserved.

Contributors to the SynEdit and mwEdit projects are listed in the
Contributors.txt file.

Alternatively, the contents of this file may be used under the terms of the
GNU General Public License Version 2 or later (the "GPL"), in which case
the provisions of the GPL are applicable instead of those above.
If you wish to allow use of your version of this file only under the terms
of the GPL and not to allow others to use your version of this file
under the MPL, indicate your decision by deleting the provisions above and
replace them with the notice and other provisions required by the GPL.
If you do not delete the provisions above, a recipient may use your version
of this file under either the MPL or the GPL.

$Id: $

You may retrieve the latest version of this file at the SynEdit home page,
located at http://SynEdit.SourceForge.net

-------------------------------------------------------------------------------}

{$IFNDEF QSYNHIGHLIGHTERHLSL}
unit SynHighlighterHLSL;
{$ENDIF}

{$I SynEdit.inc}

interface

uses
{$IFDEF SYN_CLX}
  QGraphics,
  QSynEditTypes,
  QSynEditHighlighter,
  QSynUnicode,
{$ELSE}
  Graphics,
  SynEditTypes,
  SynEditHighlighter,
  SynUnicode,
{$ENDIF}
  SysUtils,
  Classes;

type
  TtkTokenKind = (
    tkBuiltInTypes,
    tkComment,
    tkControlFlow,
    tkFuncs,
    tkIdentifier,
    tkKey,
    tkMiscKey,
    tkNull,
    tkObjs,
    tkSemantic,
    tkShaderAttr,
    tkSpace,
    tkStorageClass,
    tkString,
    tkTypeMod,
    tkUnknown,
    tkVarUsage);

  TRangeState = (rsUnKnown, rsCStyleComment, rsString);

  TProcTableProc = procedure of object;

  PIdentFuncTableFunc = ^TIdentFuncTableFunc;
  TIdentFuncTableFunc = function (Index: Integer): TtkTokenKind of object;

type
  TSynHLSLSyn = class(TSynCustomHighlighter)
  private
    fRange: TRangeState;
    fTokenID: TtkTokenKind;
    fIdentFuncTable: array[0..14532] of TIdentFuncTableFunc;
    fBuiltInTypesAttri: TSynHighlighterAttributes;
    fCommentAttri: TSynHighlighterAttributes;
    fControlFlowAttri: TSynHighlighterAttributes;
    fFuncsAttri: TSynHighlighterAttributes;
    fIdentifierAttri: TSynHighlighterAttributes;
    fKeyAttri: TSynHighlighterAttributes;
    fMiscKeyAttri: TSynHighlighterAttributes;
    fObjsAttri: TSynHighlighterAttributes;
    fSemanticAttri: TSynHighlighterAttributes;
    fShaderAttrAttri: TSynHighlighterAttributes;
    fSpaceAttri: TSynHighlighterAttributes;
    fStorageClassAttri: TSynHighlighterAttributes;
    fStringAttri: TSynHighlighterAttributes;
    fTypeModAttri: TSynHighlighterAttributes;
    fVarUsageAttri: TSynHighlighterAttributes;
    function HashKey(Str: PWideChar): Cardinal;
    function FuncAbort(Index: Integer): TtkTokenKind;
    function FuncAbs(Index: Integer): TtkTokenKind;
    function FuncAcos(Index: Integer): TtkTokenKind;
    function FuncAll(Index: Integer): TtkTokenKind;
    function FuncAllmemorybarrier(Index: Integer): TtkTokenKind;
    function FuncAllmemorybarrierwithgroupsync(Index: Integer): TtkTokenKind;
    function FuncAny(Index: Integer): TtkTokenKind;
    function FuncAppendstructuredbuffer(Index: Integer): TtkTokenKind;
    function FuncAsdouble(Index: Integer): TtkTokenKind;
    function FuncAsfloat(Index: Integer): TtkTokenKind;
    function FuncAsin(Index: Integer): TtkTokenKind;
    function FuncAsint(Index: Integer): TtkTokenKind;
    function FuncAsuint(Index: Integer): TtkTokenKind;
    function FuncAtan(Index: Integer): TtkTokenKind;
    function FuncAtan2(Index: Integer): TtkTokenKind;
    function FuncBinormal(Index: Integer): TtkTokenKind;
    function FuncBinormal0(Index: Integer): TtkTokenKind;
    function FuncBinormal1(Index: Integer): TtkTokenKind;
    function FuncBinormal2(Index: Integer): TtkTokenKind;
    function FuncBinormal3(Index: Integer): TtkTokenKind;
    function FuncBinormal4(Index: Integer): TtkTokenKind;
    function FuncBinormal5(Index: Integer): TtkTokenKind;
    function FuncBinormal6(Index: Integer): TtkTokenKind;
    function FuncBinormal7(Index: Integer): TtkTokenKind;
    function FuncBinormal8(Index: Integer): TtkTokenKind;
    function FuncBinormal9(Index: Integer): TtkTokenKind;
    function FuncBlendindices(Index: Integer): TtkTokenKind;
    function FuncBlendindices0(Index: Integer): TtkTokenKind;
    function FuncBlendindices1(Index: Integer): TtkTokenKind;
    function FuncBlendindices2(Index: Integer): TtkTokenKind;
    function FuncBlendindices3(Index: Integer): TtkTokenKind;
    function FuncBlendindices4(Index: Integer): TtkTokenKind;
    function FuncBlendindices5(Index: Integer): TtkTokenKind;
    function FuncBlendindices6(Index: Integer): TtkTokenKind;
    function FuncBlendindices7(Index: Integer): TtkTokenKind;
    function FuncBlendindices8(Index: Integer): TtkTokenKind;
    function FuncBlendindices9(Index: Integer): TtkTokenKind;
    function FuncBlendweight(Index: Integer): TtkTokenKind;
    function FuncBlendweight0(Index: Integer): TtkTokenKind;
    function FuncBlendweight1(Index: Integer): TtkTokenKind;
    function FuncBlendweight2(Index: Integer): TtkTokenKind;
    function FuncBlendweight3(Index: Integer): TtkTokenKind;
    function FuncBlendweight4(Index: Integer): TtkTokenKind;
    function FuncBlendweight5(Index: Integer): TtkTokenKind;
    function FuncBlendweight6(Index: Integer): TtkTokenKind;
    function FuncBlendweight7(Index: Integer): TtkTokenKind;
    function FuncBlendweight8(Index: Integer): TtkTokenKind;
    function FuncBlendweight9(Index: Integer): TtkTokenKind;
    function FuncBool(Index: Integer): TtkTokenKind;
    function FuncBool1(Index: Integer): TtkTokenKind;
    function FuncBool1x1(Index: Integer): TtkTokenKind;
    function FuncBool1x2(Index: Integer): TtkTokenKind;
    function FuncBool1x3(Index: Integer): TtkTokenKind;
    function FuncBool1x4(Index: Integer): TtkTokenKind;
    function FuncBool2(Index: Integer): TtkTokenKind;
    function FuncBool2x1(Index: Integer): TtkTokenKind;
    function FuncBool2x2(Index: Integer): TtkTokenKind;
    function FuncBool2x3(Index: Integer): TtkTokenKind;
    function FuncBool2x4(Index: Integer): TtkTokenKind;
    function FuncBool3(Index: Integer): TtkTokenKind;
    function FuncBool3x1(Index: Integer): TtkTokenKind;
    function FuncBool3x2(Index: Integer): TtkTokenKind;
    function FuncBool3x3(Index: Integer): TtkTokenKind;
    function FuncBool3x4(Index: Integer): TtkTokenKind;
    function FuncBool4(Index: Integer): TtkTokenKind;
    function FuncBool4x1(Index: Integer): TtkTokenKind;
    function FuncBool4x2(Index: Integer): TtkTokenKind;
    function FuncBool4x3(Index: Integer): TtkTokenKind;
    function FuncBool4x4(Index: Integer): TtkTokenKind;
    function FuncBranch(Index: Integer): TtkTokenKind;
    function FuncBreak(Index: Integer): TtkTokenKind;
    function FuncBuffer(Index: Integer): TtkTokenKind;
    function FuncByteaddressbuffer(Index: Integer): TtkTokenKind;
    function FuncCall(Index: Integer): TtkTokenKind;
    function FuncCase(Index: Integer): TtkTokenKind;
    function FuncCbuffer(Index: Integer): TtkTokenKind;
    function FuncCeil(Index: Integer): TtkTokenKind;
    function FuncCentroid(Index: Integer): TtkTokenKind;
    function FuncCheckaccessfullymapped(Index: Integer): TtkTokenKind;
    function FuncClamp(Index: Integer): TtkTokenKind;
    function FuncClip(Index: Integer): TtkTokenKind;
    function FuncColor(Index: Integer): TtkTokenKind;
    function FuncColor0(Index: Integer): TtkTokenKind;
    function FuncColor1(Index: Integer): TtkTokenKind;
    function FuncColor2(Index: Integer): TtkTokenKind;
    function FuncColor3(Index: Integer): TtkTokenKind;
    function FuncColor4(Index: Integer): TtkTokenKind;
    function FuncColor5(Index: Integer): TtkTokenKind;
    function FuncColor6(Index: Integer): TtkTokenKind;
    function FuncColor7(Index: Integer): TtkTokenKind;
    function FuncColor8(Index: Integer): TtkTokenKind;
    function FuncColor9(Index: Integer): TtkTokenKind;
    function FuncColumn95major(Index: Integer): TtkTokenKind;
    function FuncConst(Index: Integer): TtkTokenKind;
    function FuncConsumestructuredbuffer(Index: Integer): TtkTokenKind;
    function FuncContinue(Index: Integer): TtkTokenKind;
    function FuncCos(Index: Integer): TtkTokenKind;
    function FuncCosh(Index: Integer): TtkTokenKind;
    function FuncCountbits(Index: Integer): TtkTokenKind;
    function FuncCross(Index: Integer): TtkTokenKind;
    function FuncD3dcolortoubyte4(Index: Integer): TtkTokenKind;
    function FuncDdx(Index: Integer): TtkTokenKind;
    function FuncDdx95coarse(Index: Integer): TtkTokenKind;
    function FuncDdx95fine(Index: Integer): TtkTokenKind;
    function FuncDdy(Index: Integer): TtkTokenKind;
    function FuncDdy95coarse(Index: Integer): TtkTokenKind;
    function FuncDdy95fine(Index: Integer): TtkTokenKind;
    function FuncDefault(Index: Integer): TtkTokenKind;
    function FuncDegrees(Index: Integer): TtkTokenKind;
    function FuncDepth(Index: Integer): TtkTokenKind;
    function FuncDepth0(Index: Integer): TtkTokenKind;
    function FuncDepth1(Index: Integer): TtkTokenKind;
    function FuncDepth2(Index: Integer): TtkTokenKind;
    function FuncDepth3(Index: Integer): TtkTokenKind;
    function FuncDepth4(Index: Integer): TtkTokenKind;
    function FuncDepth5(Index: Integer): TtkTokenKind;
    function FuncDepth6(Index: Integer): TtkTokenKind;
    function FuncDepth7(Index: Integer): TtkTokenKind;
    function FuncDepth8(Index: Integer): TtkTokenKind;
    function FuncDepth9(Index: Integer): TtkTokenKind;
    function FuncDeterminant(Index: Integer): TtkTokenKind;
    function FuncDevicememorybarrier(Index: Integer): TtkTokenKind;
    function FuncDevicememorybarrierwithgroupsync(Index: Integer): TtkTokenKind;
    function FuncDiscard(Index: Integer): TtkTokenKind;
    function FuncDistance(Index: Integer): TtkTokenKind;
    function FuncDo(Index: Integer): TtkTokenKind;
    function FuncDot(Index: Integer): TtkTokenKind;
    function FuncDouble(Index: Integer): TtkTokenKind;
    function FuncDouble1(Index: Integer): TtkTokenKind;
    function FuncDouble1x1(Index: Integer): TtkTokenKind;
    function FuncDouble1x2(Index: Integer): TtkTokenKind;
    function FuncDouble1x3(Index: Integer): TtkTokenKind;
    function FuncDouble1x4(Index: Integer): TtkTokenKind;
    function FuncDouble2(Index: Integer): TtkTokenKind;
    function FuncDouble2x1(Index: Integer): TtkTokenKind;
    function FuncDouble2x2(Index: Integer): TtkTokenKind;
    function FuncDouble2x3(Index: Integer): TtkTokenKind;
    function FuncDouble2x4(Index: Integer): TtkTokenKind;
    function FuncDouble3(Index: Integer): TtkTokenKind;
    function FuncDouble3x1(Index: Integer): TtkTokenKind;
    function FuncDouble3x2(Index: Integer): TtkTokenKind;
    function FuncDouble3x3(Index: Integer): TtkTokenKind;
    function FuncDouble3x4(Index: Integer): TtkTokenKind;
    function FuncDouble4(Index: Integer): TtkTokenKind;
    function FuncDouble4x1(Index: Integer): TtkTokenKind;
    function FuncDouble4x2(Index: Integer): TtkTokenKind;
    function FuncDouble4x3(Index: Integer): TtkTokenKind;
    function FuncDouble4x4(Index: Integer): TtkTokenKind;
    function FuncDst(Index: Integer): TtkTokenKind;
    function FuncDword(Index: Integer): TtkTokenKind;
    function FuncDword1(Index: Integer): TtkTokenKind;
    function FuncDword1x1(Index: Integer): TtkTokenKind;
    function FuncDword1x2(Index: Integer): TtkTokenKind;
    function FuncDword1x3(Index: Integer): TtkTokenKind;
    function FuncDword1x4(Index: Integer): TtkTokenKind;
    function FuncDword2(Index: Integer): TtkTokenKind;
    function FuncDword2x1(Index: Integer): TtkTokenKind;
    function FuncDword2x2(Index: Integer): TtkTokenKind;
    function FuncDword2x3(Index: Integer): TtkTokenKind;
    function FuncDword2x4(Index: Integer): TtkTokenKind;
    function FuncDword3(Index: Integer): TtkTokenKind;
    function FuncDword3x1(Index: Integer): TtkTokenKind;
    function FuncDword3x2(Index: Integer): TtkTokenKind;
    function FuncDword3x3(Index: Integer): TtkTokenKind;
    function FuncDword3x4(Index: Integer): TtkTokenKind;
    function FuncDword4(Index: Integer): TtkTokenKind;
    function FuncDword4x1(Index: Integer): TtkTokenKind;
    function FuncDword4x2(Index: Integer): TtkTokenKind;
    function FuncDword4x3(Index: Integer): TtkTokenKind;
    function FuncDword4x4(Index: Integer): TtkTokenKind;
    function FuncElse(Index: Integer): TtkTokenKind;
    function FuncErrorf(Index: Integer): TtkTokenKind;
    function FuncEvaluateattributeatcentroid(Index: Integer): TtkTokenKind;
    function FuncEvaluateattributeatsample(Index: Integer): TtkTokenKind;
    function FuncEvaluateattributesnapped(Index: Integer): TtkTokenKind;
    function FuncExp(Index: Integer): TtkTokenKind;
    function FuncExp2(Index: Integer): TtkTokenKind;
    function FuncExtern(Index: Integer): TtkTokenKind;
    function FuncF16to32(Index: Integer): TtkTokenKind;
    function FuncF32to16(Index: Integer): TtkTokenKind;
    function FuncFaceforward(Index: Integer): TtkTokenKind;
    function FuncFalse(Index: Integer): TtkTokenKind;
    function FuncFirstbithigh(Index: Integer): TtkTokenKind;
    function FuncFirstbitlow(Index: Integer): TtkTokenKind;
    function FuncFlatten(Index: Integer): TtkTokenKind;
    function FuncFloat(Index: Integer): TtkTokenKind;
    function FuncFloat1(Index: Integer): TtkTokenKind;
    function FuncFloat1x1(Index: Integer): TtkTokenKind;
    function FuncFloat1x2(Index: Integer): TtkTokenKind;
    function FuncFloat1x3(Index: Integer): TtkTokenKind;
    function FuncFloat1x4(Index: Integer): TtkTokenKind;
    function FuncFloat2(Index: Integer): TtkTokenKind;
    function FuncFloat2x1(Index: Integer): TtkTokenKind;
    function FuncFloat2x2(Index: Integer): TtkTokenKind;
    function FuncFloat2x3(Index: Integer): TtkTokenKind;
    function FuncFloat2x4(Index: Integer): TtkTokenKind;
    function FuncFloat3(Index: Integer): TtkTokenKind;
    function FuncFloat3x1(Index: Integer): TtkTokenKind;
    function FuncFloat3x2(Index: Integer): TtkTokenKind;
    function FuncFloat3x3(Index: Integer): TtkTokenKind;
    function FuncFloat3x4(Index: Integer): TtkTokenKind;
    function FuncFloat4(Index: Integer): TtkTokenKind;
    function FuncFloat4x1(Index: Integer): TtkTokenKind;
    function FuncFloat4x2(Index: Integer): TtkTokenKind;
    function FuncFloat4x3(Index: Integer): TtkTokenKind;
    function FuncFloat4x4(Index: Integer): TtkTokenKind;
    function FuncFloor(Index: Integer): TtkTokenKind;
    function FuncFma(Index: Integer): TtkTokenKind;
    function FuncFmod(Index: Integer): TtkTokenKind;
    function FuncFog(Index: Integer): TtkTokenKind;
    function FuncFor(Index: Integer): TtkTokenKind;
    function FuncFrac(Index: Integer): TtkTokenKind;
    function FuncFrexp(Index: Integer): TtkTokenKind;
    function FuncFwidth(Index: Integer): TtkTokenKind;
    function FuncGetrendertargetsamplecount(Index: Integer): TtkTokenKind;
    function FuncGetrendertargetsampleposition(Index: Integer): TtkTokenKind;
    function FuncGroupmemorybarrier(Index: Integer): TtkTokenKind;
    function FuncGroupmemorybarrierwithgroupsync(Index: Integer): TtkTokenKind;
    function FuncGroupshared(Index: Integer): TtkTokenKind;
    function FuncHalf(Index: Integer): TtkTokenKind;
    function FuncHalf1(Index: Integer): TtkTokenKind;
    function FuncHalf1x1(Index: Integer): TtkTokenKind;
    function FuncHalf1x2(Index: Integer): TtkTokenKind;
    function FuncHalf1x3(Index: Integer): TtkTokenKind;
    function FuncHalf1x4(Index: Integer): TtkTokenKind;
    function FuncHalf2(Index: Integer): TtkTokenKind;
    function FuncHalf2x1(Index: Integer): TtkTokenKind;
    function FuncHalf2x2(Index: Integer): TtkTokenKind;
    function FuncHalf2x3(Index: Integer): TtkTokenKind;
    function FuncHalf2x4(Index: Integer): TtkTokenKind;
    function FuncHalf3(Index: Integer): TtkTokenKind;
    function FuncHalf3x1(Index: Integer): TtkTokenKind;
    function FuncHalf3x2(Index: Integer): TtkTokenKind;
    function FuncHalf3x3(Index: Integer): TtkTokenKind;
    function FuncHalf3x4(Index: Integer): TtkTokenKind;
    function FuncHalf4(Index: Integer): TtkTokenKind;
    function FuncHalf4x1(Index: Integer): TtkTokenKind;
    function FuncHalf4x2(Index: Integer): TtkTokenKind;
    function FuncHalf4x3(Index: Integer): TtkTokenKind;
    function FuncHalf4x4(Index: Integer): TtkTokenKind;
    function FuncIf(Index: Integer): TtkTokenKind;
    function FuncIfall(Index: Integer): TtkTokenKind;
    function FuncIfany(Index: Integer): TtkTokenKind;
    function FuncIn(Index: Integer): TtkTokenKind;
    function FuncInline(Index: Integer): TtkTokenKind;
    function FuncInout(Index: Integer): TtkTokenKind;
    function FuncInputpatch(Index: Integer): TtkTokenKind;
    function FuncInt(Index: Integer): TtkTokenKind;
    function FuncInt1(Index: Integer): TtkTokenKind;
    function FuncInt1x1(Index: Integer): TtkTokenKind;
    function FuncInt1x2(Index: Integer): TtkTokenKind;
    function FuncInt1x3(Index: Integer): TtkTokenKind;
    function FuncInt1x4(Index: Integer): TtkTokenKind;
    function FuncInt2(Index: Integer): TtkTokenKind;
    function FuncInt2x1(Index: Integer): TtkTokenKind;
    function FuncInt2x2(Index: Integer): TtkTokenKind;
    function FuncInt2x3(Index: Integer): TtkTokenKind;
    function FuncInt2x4(Index: Integer): TtkTokenKind;
    function FuncInt3(Index: Integer): TtkTokenKind;
    function FuncInt3x1(Index: Integer): TtkTokenKind;
    function FuncInt3x2(Index: Integer): TtkTokenKind;
    function FuncInt3x3(Index: Integer): TtkTokenKind;
    function FuncInt3x4(Index: Integer): TtkTokenKind;
    function FuncInt4(Index: Integer): TtkTokenKind;
    function FuncInt4x1(Index: Integer): TtkTokenKind;
    function FuncInt4x2(Index: Integer): TtkTokenKind;
    function FuncInt4x3(Index: Integer): TtkTokenKind;
    function FuncInt4x4(Index: Integer): TtkTokenKind;
    function FuncInterlockedadd(Index: Integer): TtkTokenKind;
    function FuncInterlockedand(Index: Integer): TtkTokenKind;
    function FuncInterlockedcompareexchange(Index: Integer): TtkTokenKind;
    function FuncInterlockedcomparestore(Index: Integer): TtkTokenKind;
    function FuncInterlockedexchange(Index: Integer): TtkTokenKind;
    function FuncInterlockedmax(Index: Integer): TtkTokenKind;
    function FuncInterlockedmin(Index: Integer): TtkTokenKind;
    function FuncInterlockedor(Index: Integer): TtkTokenKind;
    function FuncInterlockedxor(Index: Integer): TtkTokenKind;
    function FuncIsfinite(Index: Integer): TtkTokenKind;
    function FuncIsinf(Index: Integer): TtkTokenKind;
    function FuncIsnan(Index: Integer): TtkTokenKind;
    function FuncIsolate(Index: Integer): TtkTokenKind;
    function FuncLdexp(Index: Integer): TtkTokenKind;
    function FuncLength(Index: Integer): TtkTokenKind;
    function FuncLerp(Index: Integer): TtkTokenKind;
    function FuncLine(Index: Integer): TtkTokenKind;
    function FuncLineadj(Index: Integer): TtkTokenKind;
    function FuncLinestream(Index: Integer): TtkTokenKind;
    function FuncLit(Index: Integer): TtkTokenKind;
    function FuncLog(Index: Integer): TtkTokenKind;
    function FuncLog10(Index: Integer): TtkTokenKind;
    function FuncLog2(Index: Integer): TtkTokenKind;
    function FuncLoop(Index: Integer): TtkTokenKind;
    function FuncMad(Index: Integer): TtkTokenKind;
    function FuncMatrix(Index: Integer): TtkTokenKind;
    function FuncMax(Index: Integer): TtkTokenKind;
    function FuncMaxexports(Index: Integer): TtkTokenKind;
    function FuncMaxinstructioncount(Index: Integer): TtkTokenKind;
    function FuncMaxtempreg(Index: Integer): TtkTokenKind;
    function FuncMin(Index: Integer): TtkTokenKind;
    function FuncMin10float(Index: Integer): TtkTokenKind;
    function FuncMin10float1(Index: Integer): TtkTokenKind;
    function FuncMin10float1x1(Index: Integer): TtkTokenKind;
    function FuncMin10float1x2(Index: Integer): TtkTokenKind;
    function FuncMin10float1x3(Index: Integer): TtkTokenKind;
    function FuncMin10float1x4(Index: Integer): TtkTokenKind;
    function FuncMin10float2(Index: Integer): TtkTokenKind;
    function FuncMin10float2x1(Index: Integer): TtkTokenKind;
    function FuncMin10float2x2(Index: Integer): TtkTokenKind;
    function FuncMin10float2x3(Index: Integer): TtkTokenKind;
    function FuncMin10float2x4(Index: Integer): TtkTokenKind;
    function FuncMin10float3(Index: Integer): TtkTokenKind;
    function FuncMin10float3x1(Index: Integer): TtkTokenKind;
    function FuncMin10float3x2(Index: Integer): TtkTokenKind;
    function FuncMin10float3x3(Index: Integer): TtkTokenKind;
    function FuncMin10float3x4(Index: Integer): TtkTokenKind;
    function FuncMin10float4(Index: Integer): TtkTokenKind;
    function FuncMin10float4x1(Index: Integer): TtkTokenKind;
    function FuncMin10float4x2(Index: Integer): TtkTokenKind;
    function FuncMin10float4x3(Index: Integer): TtkTokenKind;
    function FuncMin10float4x4(Index: Integer): TtkTokenKind;
    function FuncMin12int(Index: Integer): TtkTokenKind;
    function FuncMin12int1(Index: Integer): TtkTokenKind;
    function FuncMin12int1x1(Index: Integer): TtkTokenKind;
    function FuncMin12int1x2(Index: Integer): TtkTokenKind;
    function FuncMin12int1x3(Index: Integer): TtkTokenKind;
    function FuncMin12int1x4(Index: Integer): TtkTokenKind;
    function FuncMin12int2(Index: Integer): TtkTokenKind;
    function FuncMin12int2x1(Index: Integer): TtkTokenKind;
    function FuncMin12int2x2(Index: Integer): TtkTokenKind;
    function FuncMin12int2x3(Index: Integer): TtkTokenKind;
    function FuncMin12int2x4(Index: Integer): TtkTokenKind;
    function FuncMin12int3(Index: Integer): TtkTokenKind;
    function FuncMin12int3x1(Index: Integer): TtkTokenKind;
    function FuncMin12int3x2(Index: Integer): TtkTokenKind;
    function FuncMin12int3x3(Index: Integer): TtkTokenKind;
    function FuncMin12int3x4(Index: Integer): TtkTokenKind;
    function FuncMin12int4(Index: Integer): TtkTokenKind;
    function FuncMin12int4x1(Index: Integer): TtkTokenKind;
    function FuncMin12int4x2(Index: Integer): TtkTokenKind;
    function FuncMin12int4x3(Index: Integer): TtkTokenKind;
    function FuncMin12int4x4(Index: Integer): TtkTokenKind;
    function FuncMin16float(Index: Integer): TtkTokenKind;
    function FuncMin16float1(Index: Integer): TtkTokenKind;
    function FuncMin16float1x1(Index: Integer): TtkTokenKind;
    function FuncMin16float1x2(Index: Integer): TtkTokenKind;
    function FuncMin16float1x3(Index: Integer): TtkTokenKind;
    function FuncMin16float1x4(Index: Integer): TtkTokenKind;
    function FuncMin16float2(Index: Integer): TtkTokenKind;
    function FuncMin16float2x1(Index: Integer): TtkTokenKind;
    function FuncMin16float2x2(Index: Integer): TtkTokenKind;
    function FuncMin16float2x3(Index: Integer): TtkTokenKind;
    function FuncMin16float2x4(Index: Integer): TtkTokenKind;
    function FuncMin16float3(Index: Integer): TtkTokenKind;
    function FuncMin16float3x1(Index: Integer): TtkTokenKind;
    function FuncMin16float3x2(Index: Integer): TtkTokenKind;
    function FuncMin16float3x3(Index: Integer): TtkTokenKind;
    function FuncMin16float3x4(Index: Integer): TtkTokenKind;
    function FuncMin16float4(Index: Integer): TtkTokenKind;
    function FuncMin16float4x1(Index: Integer): TtkTokenKind;
    function FuncMin16float4x2(Index: Integer): TtkTokenKind;
    function FuncMin16float4x3(Index: Integer): TtkTokenKind;
    function FuncMin16float4x4(Index: Integer): TtkTokenKind;
    function FuncMin16int(Index: Integer): TtkTokenKind;
    function FuncMin16int1(Index: Integer): TtkTokenKind;
    function FuncMin16int1x1(Index: Integer): TtkTokenKind;
    function FuncMin16int1x2(Index: Integer): TtkTokenKind;
    function FuncMin16int1x3(Index: Integer): TtkTokenKind;
    function FuncMin16int1x4(Index: Integer): TtkTokenKind;
    function FuncMin16int2(Index: Integer): TtkTokenKind;
    function FuncMin16int2x1(Index: Integer): TtkTokenKind;
    function FuncMin16int2x2(Index: Integer): TtkTokenKind;
    function FuncMin16int2x3(Index: Integer): TtkTokenKind;
    function FuncMin16int2x4(Index: Integer): TtkTokenKind;
    function FuncMin16int3(Index: Integer): TtkTokenKind;
    function FuncMin16int3x1(Index: Integer): TtkTokenKind;
    function FuncMin16int3x2(Index: Integer): TtkTokenKind;
    function FuncMin16int3x3(Index: Integer): TtkTokenKind;
    function FuncMin16int3x4(Index: Integer): TtkTokenKind;
    function FuncMin16int4(Index: Integer): TtkTokenKind;
    function FuncMin16int4x1(Index: Integer): TtkTokenKind;
    function FuncMin16int4x2(Index: Integer): TtkTokenKind;
    function FuncMin16int4x3(Index: Integer): TtkTokenKind;
    function FuncMin16int4x4(Index: Integer): TtkTokenKind;
    function FuncMin16uint(Index: Integer): TtkTokenKind;
    function FuncMin16uint1(Index: Integer): TtkTokenKind;
    function FuncMin16uint1x1(Index: Integer): TtkTokenKind;
    function FuncMin16uint1x2(Index: Integer): TtkTokenKind;
    function FuncMin16uint1x3(Index: Integer): TtkTokenKind;
    function FuncMin16uint1x4(Index: Integer): TtkTokenKind;
    function FuncMin16uint2(Index: Integer): TtkTokenKind;
    function FuncMin16uint2x1(Index: Integer): TtkTokenKind;
    function FuncMin16uint2x2(Index: Integer): TtkTokenKind;
    function FuncMin16uint2x3(Index: Integer): TtkTokenKind;
    function FuncMin16uint2x4(Index: Integer): TtkTokenKind;
    function FuncMin16uint3(Index: Integer): TtkTokenKind;
    function FuncMin16uint3x1(Index: Integer): TtkTokenKind;
    function FuncMin16uint3x2(Index: Integer): TtkTokenKind;
    function FuncMin16uint3x3(Index: Integer): TtkTokenKind;
    function FuncMin16uint3x4(Index: Integer): TtkTokenKind;
    function FuncMin16uint4(Index: Integer): TtkTokenKind;
    function FuncMin16uint4x1(Index: Integer): TtkTokenKind;
    function FuncMin16uint4x2(Index: Integer): TtkTokenKind;
    function FuncMin16uint4x3(Index: Integer): TtkTokenKind;
    function FuncMin16uint4x4(Index: Integer): TtkTokenKind;
    function FuncModf(Index: Integer): TtkTokenKind;
    function FuncMsad4(Index: Integer): TtkTokenKind;
    function FuncMul(Index: Integer): TtkTokenKind;
    function FuncNamespace(Index: Integer): TtkTokenKind;
    function FuncNoexpressionoptimizations(Index: Integer): TtkTokenKind;
    function FuncNointerpolation(Index: Integer): TtkTokenKind;
    function FuncNoise(Index: Integer): TtkTokenKind;
    function FuncNormal(Index: Integer): TtkTokenKind;
    function FuncNormal0(Index: Integer): TtkTokenKind;
    function FuncNormal1(Index: Integer): TtkTokenKind;
    function FuncNormal2(Index: Integer): TtkTokenKind;
    function FuncNormal3(Index: Integer): TtkTokenKind;
    function FuncNormal4(Index: Integer): TtkTokenKind;
    function FuncNormal5(Index: Integer): TtkTokenKind;
    function FuncNormal6(Index: Integer): TtkTokenKind;
    function FuncNormal7(Index: Integer): TtkTokenKind;
    function FuncNormal8(Index: Integer): TtkTokenKind;
    function FuncNormal9(Index: Integer): TtkTokenKind;
    function FuncNormalize(Index: Integer): TtkTokenKind;
    function FuncNumthreads(Index: Integer): TtkTokenKind;
    function FuncOut(Index: Integer): TtkTokenKind;
    function FuncOutputpatch(Index: Integer): TtkTokenKind;
    function FuncPackoffset(Index: Integer): TtkTokenKind;
    function FuncPoint(Index: Integer): TtkTokenKind;
    function FuncPointstream(Index: Integer): TtkTokenKind;
    function FuncPosition(Index: Integer): TtkTokenKind;
    function FuncPosition0(Index: Integer): TtkTokenKind;
    function FuncPosition1(Index: Integer): TtkTokenKind;
    function FuncPosition2(Index: Integer): TtkTokenKind;
    function FuncPosition3(Index: Integer): TtkTokenKind;
    function FuncPosition4(Index: Integer): TtkTokenKind;
    function FuncPosition5(Index: Integer): TtkTokenKind;
    function FuncPosition6(Index: Integer): TtkTokenKind;
    function FuncPosition7(Index: Integer): TtkTokenKind;
    function FuncPosition8(Index: Integer): TtkTokenKind;
    function FuncPosition9(Index: Integer): TtkTokenKind;
    function FuncPositiont(Index: Integer): TtkTokenKind;
    function FuncPow(Index: Integer): TtkTokenKind;
    function FuncPrecise(Index: Integer): TtkTokenKind;
    function FuncPredicate(Index: Integer): TtkTokenKind;
    function FuncPredicateblock(Index: Integer): TtkTokenKind;
    function FuncPrintf(Index: Integer): TtkTokenKind;
    function FuncProcess2dquadtessfactorsavg(Index: Integer): TtkTokenKind;
    function FuncProcess2dquadtessfactorsmax(Index: Integer): TtkTokenKind;
    function FuncProcess2dquadtessfactorsmin(Index: Integer): TtkTokenKind;
    function FuncProcessisolinetessfactors(Index: Integer): TtkTokenKind;
    function FuncProcessquadtessfactorsavg(Index: Integer): TtkTokenKind;
    function FuncProcessquadtessfactorsmax(Index: Integer): TtkTokenKind;
    function FuncProcessquadtessfactorsmin(Index: Integer): TtkTokenKind;
    function FuncProcesstritessfactorsavg(Index: Integer): TtkTokenKind;
    function FuncProcesstritessfactorsmax(Index: Integer): TtkTokenKind;
    function FuncProcesstritessfactorsmin(Index: Integer): TtkTokenKind;
    function FuncPsize(Index: Integer): TtkTokenKind;
    function FuncPsize0(Index: Integer): TtkTokenKind;
    function FuncPsize1(Index: Integer): TtkTokenKind;
    function FuncPsize2(Index: Integer): TtkTokenKind;
    function FuncPsize3(Index: Integer): TtkTokenKind;
    function FuncPsize4(Index: Integer): TtkTokenKind;
    function FuncPsize5(Index: Integer): TtkTokenKind;
    function FuncPsize6(Index: Integer): TtkTokenKind;
    function FuncPsize7(Index: Integer): TtkTokenKind;
    function FuncPsize8(Index: Integer): TtkTokenKind;
    function FuncPsize9(Index: Integer): TtkTokenKind;
    function FuncRadians(Index: Integer): TtkTokenKind;
    function FuncRcp(Index: Integer): TtkTokenKind;
    function FuncReducetempregusage(Index: Integer): TtkTokenKind;
    function FuncReflect(Index: Integer): TtkTokenKind;
    function FuncRefract(Index: Integer): TtkTokenKind;
    function FuncRegister(Index: Integer): TtkTokenKind;
    function FuncRemoveunusedinputs(Index: Integer): TtkTokenKind;
    function FuncReturn(Index: Integer): TtkTokenKind;
    function FuncReversebits(Index: Integer): TtkTokenKind;
    function FuncRound(Index: Integer): TtkTokenKind;
    function FuncRow95major(Index: Integer): TtkTokenKind;
    function FuncRsqrt(Index: Integer): TtkTokenKind;
    function FuncRwbuffer(Index: Integer): TtkTokenKind;
    function FuncRwbyteaddressbuffer(Index: Integer): TtkTokenKind;
    function FuncRwstructuredbuffer(Index: Integer): TtkTokenKind;
    function FuncRwtexture1d(Index: Integer): TtkTokenKind;
    function FuncRwtexture1darray(Index: Integer): TtkTokenKind;
    function FuncRwtexture2d(Index: Integer): TtkTokenKind;
    function FuncRwtexture2darray(Index: Integer): TtkTokenKind;
    function FuncRwtexture3d(Index: Integer): TtkTokenKind;
    function FuncSample(Index: Integer): TtkTokenKind;
    function FuncSampler(Index: Integer): TtkTokenKind;
    function FuncSamplercomparisonstate(Index: Integer): TtkTokenKind;
    function FuncSamplerstate(Index: Integer): TtkTokenKind;
    function FuncSampreg(Index: Integer): TtkTokenKind;
    function FuncSaturate(Index: Integer): TtkTokenKind;
    function FuncShared(Index: Integer): TtkTokenKind;
    function FuncSign(Index: Integer): TtkTokenKind;
    function FuncSin(Index: Integer): TtkTokenKind;
    function FuncSincos(Index: Integer): TtkTokenKind;
    function FuncSinh(Index: Integer): TtkTokenKind;
    function FuncSmoothstep(Index: Integer): TtkTokenKind;
    function FuncSnorm(Index: Integer): TtkTokenKind;
    function FuncSqrt(Index: Integer): TtkTokenKind;
    function FuncStatic(Index: Integer): TtkTokenKind;
    function FuncStep(Index: Integer): TtkTokenKind;
    function FuncStruct(Index: Integer): TtkTokenKind;
    function FuncStructuredbuffer(Index: Integer): TtkTokenKind;
    function FuncSv95clipdistance(Index: Integer): TtkTokenKind;
    function FuncSv95clipdistance0(Index: Integer): TtkTokenKind;
    function FuncSv95clipdistance1(Index: Integer): TtkTokenKind;
    function FuncSv95clipdistance2(Index: Integer): TtkTokenKind;
    function FuncSv95clipdistance3(Index: Integer): TtkTokenKind;
    function FuncSv95clipdistance4(Index: Integer): TtkTokenKind;
    function FuncSv95clipdistance5(Index: Integer): TtkTokenKind;
    function FuncSv95clipdistance6(Index: Integer): TtkTokenKind;
    function FuncSv95clipdistance7(Index: Integer): TtkTokenKind;
    function FuncSv95clipdistance8(Index: Integer): TtkTokenKind;
    function FuncSv95clipdistance9(Index: Integer): TtkTokenKind;
    function FuncSv95coverage(Index: Integer): TtkTokenKind;
    function FuncSv95culldistance(Index: Integer): TtkTokenKind;
    function FuncSv95culldistance0(Index: Integer): TtkTokenKind;
    function FuncSv95culldistance1(Index: Integer): TtkTokenKind;
    function FuncSv95culldistance2(Index: Integer): TtkTokenKind;
    function FuncSv95culldistance3(Index: Integer): TtkTokenKind;
    function FuncSv95culldistance4(Index: Integer): TtkTokenKind;
    function FuncSv95culldistance5(Index: Integer): TtkTokenKind;
    function FuncSv95culldistance6(Index: Integer): TtkTokenKind;
    function FuncSv95culldistance7(Index: Integer): TtkTokenKind;
    function FuncSv95culldistance8(Index: Integer): TtkTokenKind;
    function FuncSv95culldistance9(Index: Integer): TtkTokenKind;
    function FuncSv95depth(Index: Integer): TtkTokenKind;
    function FuncSv95dispatchthreadid(Index: Integer): TtkTokenKind;
    function FuncSv95domainlocation(Index: Integer): TtkTokenKind;
    function FuncSv95groupid(Index: Integer): TtkTokenKind;
    function FuncSv95groupindex(Index: Integer): TtkTokenKind;
    function FuncSv95groupthreadid(Index: Integer): TtkTokenKind;
    function FuncSv95gsinstanceid(Index: Integer): TtkTokenKind;
    function FuncSv95insidetessfactor(Index: Integer): TtkTokenKind;
    function FuncSv95instanceid(Index: Integer): TtkTokenKind;
    function FuncSv95isfrontface(Index: Integer): TtkTokenKind;
    function FuncSv95outputcontrolpointid(Index: Integer): TtkTokenKind;
    function FuncSv95position(Index: Integer): TtkTokenKind;
    function FuncSv95primitiveid(Index: Integer): TtkTokenKind;
    function FuncSv95rendertargetarrayindex(Index: Integer): TtkTokenKind;
    function FuncSv95sampleindex(Index: Integer): TtkTokenKind;
    function FuncSv95target(Index: Integer): TtkTokenKind;
    function FuncSv95target0(Index: Integer): TtkTokenKind;
    function FuncSv95target1(Index: Integer): TtkTokenKind;
    function FuncSv95target2(Index: Integer): TtkTokenKind;
    function FuncSv95target3(Index: Integer): TtkTokenKind;
    function FuncSv95target4(Index: Integer): TtkTokenKind;
    function FuncSv95target5(Index: Integer): TtkTokenKind;
    function FuncSv95target6(Index: Integer): TtkTokenKind;
    function FuncSv95target7(Index: Integer): TtkTokenKind;
    function FuncSv95tessfactor(Index: Integer): TtkTokenKind;
    function FuncSv95vertexid(Index: Integer): TtkTokenKind;
    function FuncSv95viewportarrayindex(Index: Integer): TtkTokenKind;
    function FuncSwitch(Index: Integer): TtkTokenKind;
    function FuncTan(Index: Integer): TtkTokenKind;
    function FuncTangent(Index: Integer): TtkTokenKind;
    function FuncTangent0(Index: Integer): TtkTokenKind;
    function FuncTangent1(Index: Integer): TtkTokenKind;
    function FuncTangent2(Index: Integer): TtkTokenKind;
    function FuncTangent3(Index: Integer): TtkTokenKind;
    function FuncTangent4(Index: Integer): TtkTokenKind;
    function FuncTangent5(Index: Integer): TtkTokenKind;
    function FuncTangent6(Index: Integer): TtkTokenKind;
    function FuncTangent7(Index: Integer): TtkTokenKind;
    function FuncTangent8(Index: Integer): TtkTokenKind;
    function FuncTangent9(Index: Integer): TtkTokenKind;
    function FuncTanh(Index: Integer): TtkTokenKind;
    function FuncTbuffer(Index: Integer): TtkTokenKind;
    function FuncTessfactor(Index: Integer): TtkTokenKind;
    function FuncTessfactor0(Index: Integer): TtkTokenKind;
    function FuncTessfactor1(Index: Integer): TtkTokenKind;
    function FuncTessfactor2(Index: Integer): TtkTokenKind;
    function FuncTessfactor3(Index: Integer): TtkTokenKind;
    function FuncTessfactor4(Index: Integer): TtkTokenKind;
    function FuncTessfactor5(Index: Integer): TtkTokenKind;
    function FuncTessfactor6(Index: Integer): TtkTokenKind;
    function FuncTessfactor7(Index: Integer): TtkTokenKind;
    function FuncTessfactor8(Index: Integer): TtkTokenKind;
    function FuncTessfactor9(Index: Integer): TtkTokenKind;
    function FuncTex1d(Index: Integer): TtkTokenKind;
    function FuncTex1dbias(Index: Integer): TtkTokenKind;
    function FuncTex1dgrad(Index: Integer): TtkTokenKind;
    function FuncTex1dlod(Index: Integer): TtkTokenKind;
    function FuncTex1dproj(Index: Integer): TtkTokenKind;
    function FuncTex2d(Index: Integer): TtkTokenKind;
    function FuncTex2dbias(Index: Integer): TtkTokenKind;
    function FuncTex2dgrad(Index: Integer): TtkTokenKind;
    function FuncTex2dlod(Index: Integer): TtkTokenKind;
    function FuncTex2dproj(Index: Integer): TtkTokenKind;
    function FuncTex3d(Index: Integer): TtkTokenKind;
    function FuncTex3dbias(Index: Integer): TtkTokenKind;
    function FuncTex3dgrad(Index: Integer): TtkTokenKind;
    function FuncTex3dlod(Index: Integer): TtkTokenKind;
    function FuncTex3dproj(Index: Integer): TtkTokenKind;
    function FuncTexcoord(Index: Integer): TtkTokenKind;
    function FuncTexcoord0(Index: Integer): TtkTokenKind;
    function FuncTexcoord1(Index: Integer): TtkTokenKind;
    function FuncTexcoord2(Index: Integer): TtkTokenKind;
    function FuncTexcoord3(Index: Integer): TtkTokenKind;
    function FuncTexcoord4(Index: Integer): TtkTokenKind;
    function FuncTexcoord5(Index: Integer): TtkTokenKind;
    function FuncTexcoord6(Index: Integer): TtkTokenKind;
    function FuncTexcoord7(Index: Integer): TtkTokenKind;
    function FuncTexcoord8(Index: Integer): TtkTokenKind;
    function FuncTexcoord9(Index: Integer): TtkTokenKind;
    function FuncTexcube(Index: Integer): TtkTokenKind;
    function FuncTexcubebias(Index: Integer): TtkTokenKind;
    function FuncTexcubegrad(Index: Integer): TtkTokenKind;
    function FuncTexcubelod(Index: Integer): TtkTokenKind;
    function FuncTexcubeproj(Index: Integer): TtkTokenKind;
    function FuncTexture1d(Index: Integer): TtkTokenKind;
    function FuncTexture1darray(Index: Integer): TtkTokenKind;
    function FuncTexture2d(Index: Integer): TtkTokenKind;
    function FuncTexture2darray(Index: Integer): TtkTokenKind;
    function FuncTexture2dms(Index: Integer): TtkTokenKind;
    function FuncTexture2dmsarray(Index: Integer): TtkTokenKind;
    function FuncTexture3d(Index: Integer): TtkTokenKind;
    function FuncTexturecube(Index: Integer): TtkTokenKind;
    function FuncTexturecubearray(Index: Integer): TtkTokenKind;
    function FuncTranspose(Index: Integer): TtkTokenKind;
    function FuncTrianglestream(Index: Integer): TtkTokenKind;
    function FuncTrue(Index: Integer): TtkTokenKind;
    function FuncTrunc(Index: Integer): TtkTokenKind;
    function FuncUint(Index: Integer): TtkTokenKind;
    function FuncUint1(Index: Integer): TtkTokenKind;
    function FuncUint1x1(Index: Integer): TtkTokenKind;
    function FuncUint1x2(Index: Integer): TtkTokenKind;
    function FuncUint1x3(Index: Integer): TtkTokenKind;
    function FuncUint1x4(Index: Integer): TtkTokenKind;
    function FuncUint2(Index: Integer): TtkTokenKind;
    function FuncUint2x1(Index: Integer): TtkTokenKind;
    function FuncUint2x2(Index: Integer): TtkTokenKind;
    function FuncUint2x3(Index: Integer): TtkTokenKind;
    function FuncUint2x4(Index: Integer): TtkTokenKind;
    function FuncUint3(Index: Integer): TtkTokenKind;
    function FuncUint3x1(Index: Integer): TtkTokenKind;
    function FuncUint3x2(Index: Integer): TtkTokenKind;
    function FuncUint3x3(Index: Integer): TtkTokenKind;
    function FuncUint3x4(Index: Integer): TtkTokenKind;
    function FuncUint4(Index: Integer): TtkTokenKind;
    function FuncUint4x1(Index: Integer): TtkTokenKind;
    function FuncUint4x2(Index: Integer): TtkTokenKind;
    function FuncUint4x3(Index: Integer): TtkTokenKind;
    function FuncUint4x4(Index: Integer): TtkTokenKind;
    function FuncUniform(Index: Integer): TtkTokenKind;
    function FuncUnorm(Index: Integer): TtkTokenKind;
    function FuncUnroll(Index: Integer): TtkTokenKind;
    function FuncUnsigned(Index: Integer): TtkTokenKind;
    function FuncUnused(Index: Integer): TtkTokenKind;
    function FuncVector(Index: Integer): TtkTokenKind;
    function FuncVoid(Index: Integer): TtkTokenKind;
    function FuncVolatile(Index: Integer): TtkTokenKind;
    function FuncWhile(Index: Integer): TtkTokenKind;
    procedure IdentProc;
    procedure UnknownProc;
    function AltFunc(Index: Integer): TtkTokenKind;
    procedure InitIdent;
    function IdentKind(MayBe: PWideChar): TtkTokenKind;
    procedure NullProc;
    procedure SpaceProc;
    procedure CRProc;
    procedure LFProc;
    procedure CStyleCommentOpenProc;
    procedure CStyleCommentProc;
    procedure StringOpenProc;
    procedure StringProc;
  protected
    function GetSampleSource: UnicodeString; override;
    function IsFilterStored: Boolean; override;
  public
    constructor Create(AOwner: TComponent); override;
    class function GetFriendlyLanguageName: UnicodeString; override;
    class function GetLanguageName: string; override;
    function GetRange: Pointer; override;
    procedure ResetRange; override;
    procedure SetRange(Value: Pointer); override;
    function GetDefaultAttribute(Index: Integer): TSynHighlighterAttributes; override;
    function GetEol: Boolean; override;
    function GetKeyWords(TokenKind: Integer): UnicodeString; override;
    function GetTokenID: TtkTokenKind;
    function GetTokenAttribute: TSynHighlighterAttributes; override;
    function GetTokenKind: Integer; override;
    function IsIdentChar(AChar: WideChar): Boolean; override;
    procedure Next; override;
  published
    property BuiltInTypesAttri: TSynHighlighterAttributes read fBuiltInTypesAttri write fBuiltInTypesAttri;
    property CommentAttri: TSynHighlighterAttributes read fCommentAttri write fCommentAttri;
    property ControlFlowAttri: TSynHighlighterAttributes read fControlFlowAttri write fControlFlowAttri;
    property FuncsAttri: TSynHighlighterAttributes read fFuncsAttri write fFuncsAttri;
    property IdentifierAttri: TSynHighlighterAttributes read fIdentifierAttri write fIdentifierAttri;
    property KeyAttri: TSynHighlighterAttributes read fKeyAttri write fKeyAttri;
    property MiscKeyAttri: TSynHighlighterAttributes read fMiscKeyAttri write fMiscKeyAttri;
    property ObjsAttri: TSynHighlighterAttributes read fObjsAttri write fObjsAttri;
    property SemanticAttri: TSynHighlighterAttributes read fSemanticAttri write fSemanticAttri;
    property ShaderAttrAttri: TSynHighlighterAttributes read fShaderAttrAttri write fShaderAttrAttri;
    property SpaceAttri: TSynHighlighterAttributes read fSpaceAttri write fSpaceAttri;
    property StorageClassAttri: TSynHighlighterAttributes read fStorageClassAttri write fStorageClassAttri;
    property StringAttri: TSynHighlighterAttributes read fStringAttri write fStringAttri;
    property TypeModAttri: TSynHighlighterAttributes read fTypeModAttri write fTypeModAttri;
    property VarUsageAttri: TSynHighlighterAttributes read fVarUsageAttri write fVarUsageAttri;
  end;

implementation

uses
{$IFDEF SYN_CLX}
  QSynEditStrConst;
{$ELSE}
  SynEditStrConst;
{$ENDIF}

resourcestring
  SYNS_FilterHLSL = 'HLSL files (*.h, *.hlsl, *.hlsli)|*.h;*.hlsl;*.hlsli';
  SYNS_LangHLSL = 'HLSL';
  SYNS_FriendlyLangHLSL = 'HLSL';
  SYNS_AttrBuiltInTypes = 'BuiltInTypes';
  SYNS_FriendlyAttrBuiltInTypes = 'BuiltInTypes';
  SYNS_AttrControlFlow = 'ControlFlow';
  SYNS_FriendlyAttrControlFlow = 'ControlFlow';
  SYNS_AttrFuncs = 'Funcs';
  SYNS_FriendlyAttrFuncs = 'Funcs';
  SYNS_AttrMiscKey = 'MiscKey';
  SYNS_FriendlyAttrMiscKey = 'MiscKey';
  SYNS_AttrObjs = 'Objs';
  SYNS_FriendlyAttrObjs = 'Objs';
  SYNS_AttrSemantic = 'Semantic';
  SYNS_FriendlyAttrSemantic = 'Semantic';
  SYNS_AttrShaderAttr = 'ShaderAttr';
  SYNS_FriendlyAttrShaderAttr = 'ShaderAttr';
  SYNS_AttrStorageClass = 'StorageClass';
  SYNS_FriendlyAttrStorageClass = 'StorageClass';
  SYNS_AttrTypeMod = 'TypeMod';
  SYNS_FriendlyAttrTypeMod = 'TypeMod';
  SYNS_AttrVarUsage = 'VarUsage';
  SYNS_FriendlyAttrVarUsage = 'VarUsage';

const
  // as this language is case-insensitive keywords *must* be in lowercase
  KeyWords: array[0..654] of UnicodeString = (
    'abort', 'abs', 'acos', 'all', 'allmemorybarrier', 
    'allmemorybarrierwithgroupsync', 'any', 'appendstructuredbuffer', 
    'asdouble', 'asfloat', 'asin', 'asint', 'asuint', 'atan', 'atan2', 
    'binormal', 'binormal0', 'binormal1', 'binormal2', 'binormal3', 'binormal4', 
    'binormal5', 'binormal6', 'binormal7', 'binormal8', 'binormal9', 
    'blendindices', 'blendindices0', 'blendindices1', 'blendindices2', 
    'blendindices3', 'blendindices4', 'blendindices5', 'blendindices6', 
    'blendindices7', 'blendindices8', 'blendindices9', 'blendweight', 
    'blendweight0', 'blendweight1', 'blendweight2', 'blendweight3', 
    'blendweight4', 'blendweight5', 'blendweight6', 'blendweight7', 
    'blendweight8', 'blendweight9', 'bool', 'bool1', 'bool1x1', 'bool1x2', 
    'bool1x3', 'bool1x4', 'bool2', 'bool2x1', 'bool2x2', 'bool2x3', 'bool2x4', 
    'bool3', 'bool3x1', 'bool3x2', 'bool3x3', 'bool3x4', 'bool4', 'bool4x1', 
    'bool4x2', 'bool4x3', 'bool4x4', 'branch', 'break', 'buffer', 
    'byteaddressbuffer', 'call', 'case', 'cbuffer', 'ceil', 'centroid', 
    'checkaccessfullymapped', 'clamp', 'clip', 'color', 'color0', 'color1', 
    'color2', 'color3', 'color4', 'color5', 'color6', 'color7', 'color8', 
    'color9', 'column_major', 'const', 'consumestructuredbuffer', 'continue', 
    'cos', 'cosh', 'countbits', 'cross', 'd3dcolortoubyte4', 'ddx', 
    'ddx_coarse', 'ddx_fine', 'ddy', 'ddy_coarse', 'ddy_fine', 'default', 
    'degrees', 'depth', 'depth0', 'depth1', 'depth2', 'depth3', 'depth4', 
    'depth5', 'depth6', 'depth7', 'depth8', 'depth9', 'determinant', 
    'devicememorybarrier', 'devicememorybarrierwithgroupsync', 'discard', 
    'distance', 'do', 'dot', 'double', 'double1', 'double1x1', 'double1x2', 
    'double1x3', 'double1x4', 'double2', 'double2x1', 'double2x2', 'double2x3', 
    'double2x4', 'double3', 'double3x1', 'double3x2', 'double3x3', 'double3x4', 
    'double4', 'double4x1', 'double4x2', 'double4x3', 'double4x4', 'dst', 
    'dword', 'dword1', 'dword1x1', 'dword1x2', 'dword1x3', 'dword1x4', 'dword2', 
    'dword2x1', 'dword2x2', 'dword2x3', 'dword2x4', 'dword3', 'dword3x1', 
    'dword3x2', 'dword3x3', 'dword3x4', 'dword4', 'dword4x1', 'dword4x2', 
    'dword4x3', 'dword4x4', 'else', 'errorf', 'evaluateattributeatcentroid', 
    'evaluateattributeatsample', 'evaluateattributesnapped', 'exp', 'exp2', 
    'extern', 'f16to32', 'f32to16', 'faceforward', 'false', 'firstbithigh', 
    'firstbitlow', 'flatten', 'float', 'float1', 'float1x1', 'float1x2', 
    'float1x3', 'float1x4', 'float2', 'float2x1', 'float2x2', 'float2x3', 
    'float2x4', 'float3', 'float3x1', 'float3x2', 'float3x3', 'float3x4', 
    'float4', 'float4x1', 'float4x2', 'float4x3', 'float4x4', 'floor', 'fma', 
    'fmod', 'fog', 'for', 'frac', 'frexp', 'fwidth', 
    'getrendertargetsamplecount', 'getrendertargetsampleposition', 
    'groupmemorybarrier', 'groupmemorybarrierwithgroupsync', 'groupshared', 
    'half', 'half1', 'half1x1', 'half1x2', 'half1x3', 'half1x4', 'half2', 
    'half2x1', 'half2x2', 'half2x3', 'half2x4', 'half3', 'half3x1', 'half3x2', 
    'half3x3', 'half3x4', 'half4', 'half4x1', 'half4x2', 'half4x3', 'half4x4', 
    'if', 'ifall', 'ifany', 'in', 'inline', 'inout', 'inputpatch', 'int', 
    'int1', 'int1x1', 'int1x2', 'int1x3', 'int1x4', 'int2', 'int2x1', 'int2x2', 
    'int2x3', 'int2x4', 'int3', 'int3x1', 'int3x2', 'int3x3', 'int3x4', 'int4', 
    'int4x1', 'int4x2', 'int4x3', 'int4x4', 'interlockedadd', 'interlockedand', 
    'interlockedcompareexchange', 'interlockedcomparestore', 
    'interlockedexchange', 'interlockedmax', 'interlockedmin', 'interlockedor', 
    'interlockedxor', 'isfinite', 'isinf', 'isnan', 'isolate', 'ldexp', 
    'length', 'lerp', 'line', 'lineadj', 'linestream', 'lit', 'log', 'log10', 
    'log2', 'loop', 'mad', 'matrix', 'max', 'maxexports', 'maxinstructioncount', 
    'maxtempreg', 'min', 'min10float', 'min10float1', 'min10float1x1', 
    'min10float1x2', 'min10float1x3', 'min10float1x4', 'min10float2', 
    'min10float2x1', 'min10float2x2', 'min10float2x3', 'min10float2x4', 
    'min10float3', 'min10float3x1', 'min10float3x2', 'min10float3x3', 
    'min10float3x4', 'min10float4', 'min10float4x1', 'min10float4x2', 
    'min10float4x3', 'min10float4x4', 'min12int', 'min12int1', 'min12int1x1', 
    'min12int1x2', 'min12int1x3', 'min12int1x4', 'min12int2', 'min12int2x1', 
    'min12int2x2', 'min12int2x3', 'min12int2x4', 'min12int3', 'min12int3x1', 
    'min12int3x2', 'min12int3x3', 'min12int3x4', 'min12int4', 'min12int4x1', 
    'min12int4x2', 'min12int4x3', 'min12int4x4', 'min16float', 'min16float1', 
    'min16float1x1', 'min16float1x2', 'min16float1x3', 'min16float1x4', 
    'min16float2', 'min16float2x1', 'min16float2x2', 'min16float2x3', 
    'min16float2x4', 'min16float3', 'min16float3x1', 'min16float3x2', 
    'min16float3x3', 'min16float3x4', 'min16float4', 'min16float4x1', 
    'min16float4x2', 'min16float4x3', 'min16float4x4', 'min16int', 'min16int1', 
    'min16int1x1', 'min16int1x2', 'min16int1x3', 'min16int1x4', 'min16int2', 
    'min16int2x1', 'min16int2x2', 'min16int2x3', 'min16int2x4', 'min16int3', 
    'min16int3x1', 'min16int3x2', 'min16int3x3', 'min16int3x4', 'min16int4', 
    'min16int4x1', 'min16int4x2', 'min16int4x3', 'min16int4x4', 'min16uint', 
    'min16uint1', 'min16uint1x1', 'min16uint1x2', 'min16uint1x3', 
    'min16uint1x4', 'min16uint2', 'min16uint2x1', 'min16uint2x2', 
    'min16uint2x3', 'min16uint2x4', 'min16uint3', 'min16uint3x1', 
    'min16uint3x2', 'min16uint3x3', 'min16uint3x4', 'min16uint4', 
    'min16uint4x1', 'min16uint4x2', 'min16uint4x3', 'min16uint4x4', 'modf', 
    'msad4', 'mul', 'namespace', 'noexpressionoptimizations', 'nointerpolation', 
    'noise', 'normal', 'normal0', 'normal1', 'normal2', 'normal3', 'normal4', 
    'normal5', 'normal6', 'normal7', 'normal8', 'normal9', 'normalize', 
    'numthreads', 'out', 'outputpatch', 'packoffset', 'point', 'pointstream', 
    'position', 'position0', 'position1', 'position2', 'position3', 'position4', 
    'position5', 'position6', 'position7', 'position8', 'position9', 
    'positiont', 'pow', 'precise', 'predicate', 'predicateblock', 'printf', 
    'process2dquadtessfactorsavg', 'process2dquadtessfactorsmax', 
    'process2dquadtessfactorsmin', 'processisolinetessfactors', 
    'processquadtessfactorsavg', 'processquadtessfactorsmax', 
    'processquadtessfactorsmin', 'processtritessfactorsavg', 
    'processtritessfactorsmax', 'processtritessfactorsmin', 'psize', 'psize0', 
    'psize1', 'psize2', 'psize3', 'psize4', 'psize5', 'psize6', 'psize7', 
    'psize8', 'psize9', 'radians', 'rcp', 'reducetempregusage', 'reflect', 
    'refract', 'register', 'removeunusedinputs', 'return', 'reversebits', 
    'round', 'row_major', 'rsqrt', 'rwbuffer', 'rwbyteaddressbuffer', 
    'rwstructuredbuffer', 'rwtexture1d', 'rwtexture1darray', 'rwtexture2d', 
    'rwtexture2darray', 'rwtexture3d', 'sample', 'sampler', 
    'samplercomparisonstate', 'samplerstate', 'sampreg', 'saturate', 'shared', 
    'sign', 'sin', 'sincos', 'sinh', 'smoothstep', 'snorm', 'sqrt', 'static', 
    'step', 'struct', 'structuredbuffer', 'sv_clipdistance', 'sv_clipdistance0', 
    'sv_clipdistance1', 'sv_clipdistance2', 'sv_clipdistance3', 
    'sv_clipdistance4', 'sv_clipdistance5', 'sv_clipdistance6', 
    'sv_clipdistance7', 'sv_clipdistance8', 'sv_clipdistance9', 'sv_coverage', 
    'sv_culldistance', 'sv_culldistance0', 'sv_culldistance1', 
    'sv_culldistance2', 'sv_culldistance3', 'sv_culldistance4', 
    'sv_culldistance5', 'sv_culldistance6', 'sv_culldistance7', 
    'sv_culldistance8', 'sv_culldistance9', 'sv_depth', 'sv_dispatchthreadid', 
    'sv_domainlocation', 'sv_groupid', 'sv_groupindex', 'sv_groupthreadid', 
    'sv_gsinstanceid', 'sv_insidetessfactor', 'sv_instanceid', 'sv_isfrontface', 
    'sv_outputcontrolpointid', 'sv_position', 'sv_primitiveid', 
    'sv_rendertargetarrayindex', 'sv_sampleindex', 'sv_target', 'sv_target0', 
    'sv_target1', 'sv_target2', 'sv_target3', 'sv_target4', 'sv_target5', 
    'sv_target6', 'sv_target7', 'sv_tessfactor', 'sv_vertexid', 
    'sv_viewportarrayindex', 'switch', 'tan', 'tangent', 'tangent0', 'tangent1', 
    'tangent2', 'tangent3', 'tangent4', 'tangent5', 'tangent6', 'tangent7', 
    'tangent8', 'tangent9', 'tanh', 'tbuffer', 'tessfactor', 'tessfactor0', 
    'tessfactor1', 'tessfactor2', 'tessfactor3', 'tessfactor4', 'tessfactor5', 
    'tessfactor6', 'tessfactor7', 'tessfactor8', 'tessfactor9', 'tex1d', 
    'tex1dbias', 'tex1dgrad', 'tex1dlod', 'tex1dproj', 'tex2d', 'tex2dbias', 
    'tex2dgrad', 'tex2dlod', 'tex2dproj', 'tex3d', 'tex3dbias', 'tex3dgrad', 
    'tex3dlod', 'tex3dproj', 'texcoord', 'texcoord0', 'texcoord1', 'texcoord2', 
    'texcoord3', 'texcoord4', 'texcoord5', 'texcoord6', 'texcoord7', 
    'texcoord8', 'texcoord9', 'texcube', 'texcubebias', 'texcubegrad', 
    'texcubelod', 'texcubeproj', 'texture1d', 'texture1darray', 'texture2d', 
    'texture2darray', 'texture2dms', 'texture2dmsarray', 'texture3d', 
    'texturecube', 'texturecubearray', 'transpose', 'trianglestream', 'true', 
    'trunc', 'uint', 'uint1', 'uint1x1', 'uint1x2', 'uint1x3', 'uint1x4', 
    'uint2', 'uint2x1', 'uint2x2', 'uint2x3', 'uint2x4', 'uint3', 'uint3x1', 
    'uint3x2', 'uint3x3', 'uint3x4', 'uint4', 'uint4x1', 'uint4x2', 'uint4x3', 
    'uint4x4', 'uniform', 'unorm', 'unroll', 'unsigned', 'unused', 'vector', 
    'void', 'volatile', 'while' 
  );

  KeyIndices: array[0..14532] of Integer = (
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, 448, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, 78, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    395, 396, 397, 398, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, 295, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, 353, 354, 355, 356, -1, -1, -1, -1, -1, -1, -1, -1, 269, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, 300, 305, 310, 315, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 170, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 485, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, 14, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 543, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, 76, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, 71, -1, -1, 637, 638, 639, 640, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, 445, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 625, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, 539, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, 443, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    98, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, 569, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 499, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, 277, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 592, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, 407, -1, -1, -1, 607, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, 623, -1, -1, -1, -1, -1, 477, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, 446, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    651, -1, -1, -1, -1, -1, -1, -1, -1, -1, 271, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 430, 
    431, 432, 433, 434, 435, 436, 437, 438, 439, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, 400, 401, 402, 403, -1, 440, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, 358, 359, 360, 361, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, 9, -1, -1, -1, -1, -1, -1, -1, 173, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, 426, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 272, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 424, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, 444, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 642, 643, 644, 645, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 149, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 475, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, 48, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    363, 368, 373, 378, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, 529, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 96, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 532, -1, -1, 
    -1, -1, 654, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 180, -1, 
    -1, 290, -1, -1, -1, -1, -1, -1, -1, 220, 225, 230, 235, -1, -1, -1, -1, -1, 
    211, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 441, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, 124, -1, -1, -1, -1, 214, -1, -1, 291, -1, -1, -1, 622, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 282, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 215, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 383, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 611, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, 496, -1, -1, -1, -1, -1, -1, -1, -1, -1, 362, -1, -1, -1, -1, 
    -1, -1, -1, -1, 101, 104, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 652, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 126, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 469, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, 581, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 148, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, 586, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 185, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 591, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, 219, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, 299, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 125, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 553, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, 240, -1, -1, -1, -1, -1, -1, -1, 243, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, 493, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, 275, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 456, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 206, -1, 72, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 471, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 488, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 476, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 500, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, 428, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 129, 130, 
    131, 132, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 81, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 181, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    69, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 571, 572, 573, 574, 575, 576, 
    577, 578, 579, 580, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 79, 
    -1, -1, -1, 624, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, 593, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 619, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, 289, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 322, 
    323, 324, 325, -1, -1, -1, -1, -1, -1, 468, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 590, 480, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 221, 222, 223, 224, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, 127, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 108, -1, -1, -1, -1, -1, 555, 
    -1, -1, -1, -1, 175, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, 321, 326, 331, 336, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 582, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 123, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 617, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 100, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, 151, 152, 153, 154, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 626, 631, 636, 641, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, 470, -1, -1, -1, 134, 135, 136, 137, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, 286, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 120, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 207, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, 320, -1, -1, -1, -1, -1, -1, 213, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 209, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, 210, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, 327, 328, 329, 330, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 454, -1, -1, 
    -1, 184, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 226, 227, 228, 
    229, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 283, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, 536, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 244, 
    408, -1, -1, -1, -1, -1, 8, -1, -1, -1, -1, 16, 17, 18, 19, 20, 21, 22, 23, 
    24, 25, -1, 473, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 541, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 495, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 498, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, 610, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 455, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 534, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 285, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, 528, -1, -1, -1, -1, -1, -1, 341, -1, -1, -1, -1, 621, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, 427, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 156, 157, 158, 159, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 139, 140, 141, 142, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 648, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 558, 559, 
    560, 561, 562, 563, 564, 565, 566, 567, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 93, -1, -1, -1, 
    -1, 616, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 556, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 332, 333, 334, 335, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, 171, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, 231, 232, 233, 234, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 217, 
    -1, 179, -1, -1, -1, -1, -1, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, 297, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    453, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, 502, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    530, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, 216, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    613, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    161, 162, 163, 164, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 144, 
    145, 146, 147, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 109, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 410, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 364, 
    365, 366, 367, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, 384, 389, 394, 399, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, 429, 270, -1, -1, -1, -1, -1, 245, -1, -1, -1, -1, -1, 337, 
    338, 339, 340, -1, -1, 442, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, 236, 237, 238, 239, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, 37, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 128, 
    133, 138, 143, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 542, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, 49, 54, 59, 64, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 596, -1, 
    -1, -1, -1, 178, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    554, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 489, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 533, -1, -1, -1, -1, -1, 281, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, 278, -1, 481, -1, -1, -1, -1, -1, -1, 583, -1, -1, -1, -1, 
    -1, -1, -1, 608, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, 490, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 174, 
    -1, -1, -1, -1, -1, 121, -1, -1, -1, -1, -1, -1, -1, 80, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, 246, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 166, 167, 168, 169, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 301, 302, 303, 304, -1, -1, -1, 
    -1, -1, -1, 249, 250, 251, 252, -1, -1, -1, 12, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, 95, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, 467, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, 103, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, 187, 188, 189, 190, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, 479, -1, -1, 494, -1, -1, -1, -1, 369, 370, 371, 372, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 247, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, 218, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, 409, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 497, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 122, -1, -1, -1, -1, -1, 
    -1, 412, 413, 414, 415, 416, 417, 418, 419, 420, 421, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 102, -1, -1, -1, -1, 
    -1, 612, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 614, -1, 
    535, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 618, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 538, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 99, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, 50, 51, 52, 53, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, 506, 507, 508, 509, 510, 511, 512, 513, 
    514, 515, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, 449, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, 557, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, 595, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    411, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, 306, 307, 308, 309, -1, -1, -1, -1, -1, -1, 254, 255, 256, 257, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 26, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 107, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, 552, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 192, 193, 194, 195, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 540, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 474, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, -1, -1, -1, 
    -1, -1, -1, -1, 374, 375, 376, 377, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, 568, 587, -1, -1, -1, -1, 284, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, 483, -1, 518, 519, 520, 521, 522, 523, 524, 525, 526, 527, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, 649, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 405, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 73, -1, -1, -1, -1, 
    -1, -1, -1, 212, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 422, -1, -1, 
    55, 56, 57, 58, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 74, 10, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 505, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, 491, -1, -1, -1, -1, 172, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 311, 312, 313, 314, -1, -1, -1, -1, 
    -1, -1, 259, 260, 261, 262, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 296, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, 197, 198, 199, 200, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 379, 380, 381, 382, 241, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, 242, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 482, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 478, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 484, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, 486, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    537, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 208, 
    -1, 177, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, 570, -1, -1, -1, -1, -1, -1, -1, 13, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 597, 598, 599, 600, 
    601, 602, 603, 604, 605, 606, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, 60, 61, 62, 63, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, 492, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, 609, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 293, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, 183, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, 472, -1, -1, -1, -1, -1, -1, 544, 545, 546, 547, 
    548, 549, 550, 551, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 342, 347, 352, 357, -1, -1, -1, -1, 
    -1, -1, -1, -1, 615, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 316, 
    317, 318, 319, -1, -1, -1, -1, -1, -1, 264, 265, 266, 267, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, 287, -1, 501, -1, -1, -1, 202, 203, 204, 205, -1, -1, -1, -1, 
    -1, -1, 276, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 97, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 385, 386, 387, 388, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 517, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 343, 344, 345, 346, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, 182, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, 70, 404, -1, -1, -1, -1, -1, 288, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, 650, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 27, 28, 29, 30, 31, 32, 33, 34, 35, 
    36, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 653, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, 627, 628, 629, 630, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, 594, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 65, 66, 67, 68, -1, 
    -1, -1, -1, 503, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 531, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, 94, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, 423, -1, -1, -1, -1, -1, 186, 191, 196, 201, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 105, 451, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 487, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, 292, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, 294, -1, -1, -1, 457, 458, 459, 460, 461, 462, 463, 
    464, 465, 466, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 452, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, 75, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, 298, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, 589, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    390, 391, 392, 393, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, 348, 349, 350, 351, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 588, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 273, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, 279, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 77, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    585, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 406, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1, -1, 
    150, 155, 160, 165, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, 274, -1, 425, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, 632, 633, 634, 635, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 11, -1, -1, -1, -1, -1, -1, 647, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 176, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, 106, -1, -1, -1, -1, -1, -1, 620, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, 584, -1, -1, -1, -1, -1, 3, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 6, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 447, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 92, -1, -1, 268, -1, 
    -1, -1, -1, -1, -1, 646, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 450, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, 516, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, 248, 253, 258, 263, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 280, -1, -1, 
    -1, -1, -1, -1, -1, 504, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1 
  );

procedure TSynHLSLSyn.InitIdent;
var
  i: Integer;
begin
  for i := Low(fIdentFuncTable) to High(fIdentFuncTable) do
    if KeyIndices[i] = -1 then
      fIdentFuncTable[i] := AltFunc;

  fIdentFuncTable[6823] := FuncAbort;
  fIdentFuncTable[13848] := FuncAbs;
  fIdentFuncTable[6911] := FuncAcos;
  fIdentFuncTable[14181] := FuncAll;
  fIdentFuncTable[7551] := FuncAllmemorybarrier;
  fIdentFuncTable[11821] := FuncAllmemorybarrierwithgroupsync;
  fIdentFuncTable[14262] := FuncAny;
  fIdentFuncTable[3083] := FuncAppendstructuredbuffer;
  fIdentFuncTable[5761] := FuncAsdouble;
  fIdentFuncTable[1357] := FuncAsfloat;
  fIdentFuncTable[10665] := FuncAsin;
  fIdentFuncTable[13934] := FuncAsint;
  fIdentFuncTable[8684] := FuncAsuint;
  fIdentFuncTable[11549] := FuncAtan;
  fIdentFuncTable[325] := FuncAtan2;
  fIdentFuncTable[14151] := FuncBinormal;
  fIdentFuncTable[5766] := FuncBinormal0;
  fIdentFuncTable[5767] := FuncBinormal1;
  fIdentFuncTable[5768] := FuncBinormal2;
  fIdentFuncTable[5769] := FuncBinormal3;
  fIdentFuncTable[5770] := FuncBinormal4;
  fIdentFuncTable[5771] := FuncBinormal5;
  fIdentFuncTable[5772] := FuncBinormal6;
  fIdentFuncTable[5773] := FuncBinormal7;
  fIdentFuncTable[5774] := FuncBinormal8;
  fIdentFuncTable[5775] := FuncBinormal9;
  fIdentFuncTable[9857] := FuncBlendindices;
  fIdentFuncTable[12500] := FuncBlendindices0;
  fIdentFuncTable[12501] := FuncBlendindices1;
  fIdentFuncTable[12502] := FuncBlendindices2;
  fIdentFuncTable[12503] := FuncBlendindices3;
  fIdentFuncTable[12504] := FuncBlendindices4;
  fIdentFuncTable[12505] := FuncBlendindices5;
  fIdentFuncTable[12506] := FuncBlendindices6;
  fIdentFuncTable[12507] := FuncBlendindices7;
  fIdentFuncTable[12508] := FuncBlendindices8;
  fIdentFuncTable[12509] := FuncBlendindices9;
  fIdentFuncTable[8002] := FuncBlendweight;
  fIdentFuncTable[6082] := FuncBlendweight0;
  fIdentFuncTable[6083] := FuncBlendweight1;
  fIdentFuncTable[6084] := FuncBlendweight2;
  fIdentFuncTable[6085] := FuncBlendweight3;
  fIdentFuncTable[6086] := FuncBlendweight4;
  fIdentFuncTable[6087] := FuncBlendweight5;
  fIdentFuncTable[6088] := FuncBlendweight6;
  fIdentFuncTable[6089] := FuncBlendweight7;
  fIdentFuncTable[6090] := FuncBlendweight8;
  fIdentFuncTable[6091] := FuncBlendweight9;
  fIdentFuncTable[1948] := FuncBool;
  fIdentFuncTable[8149] := FuncBool1;
  fIdentFuncTable[9415] := FuncBool1x1;
  fIdentFuncTable[9416] := FuncBool1x2;
  fIdentFuncTable[9417] := FuncBool1x3;
  fIdentFuncTable[9418] := FuncBool1x4;
  fIdentFuncTable[8150] := FuncBool2;
  fIdentFuncTable[10571] := FuncBool2x1;
  fIdentFuncTable[10572] := FuncBool2x2;
  fIdentFuncTable[10573] := FuncBool2x3;
  fIdentFuncTable[10574] := FuncBool2x4;
  fIdentFuncTable[8151] := FuncBool3;
  fIdentFuncTable[11727] := FuncBool3x1;
  fIdentFuncTable[11728] := FuncBool3x2;
  fIdentFuncTable[11729] := FuncBool3x3;
  fIdentFuncTable[11730] := FuncBool3x4;
  fIdentFuncTable[8152] := FuncBool4;
  fIdentFuncTable[12883] := FuncBool4x1;
  fIdentFuncTable[12884] := FuncBool4x2;
  fIdentFuncTable[12885] := FuncBool4x3;
  fIdentFuncTable[12886] := FuncBool4x4;
  fIdentFuncTable[4213] := FuncBranch;
  fIdentFuncTable[12454] := FuncBreak;
  fIdentFuncTable[534] := FuncBuffer;
  fIdentFuncTable[3870] := FuncByteaddressbuffer;
  fIdentFuncTable[10433] := FuncCall;
  fIdentFuncTable[10664] := FuncCase;
  fIdentFuncTable[13329] := FuncCbuffer;
  fIdentFuncTable[422] := FuncCeil;
  fIdentFuncTable[13730] := FuncCentroid;
  fIdentFuncTable[42] := FuncCheckaccessfullymapped;
  fIdentFuncTable[4247] := FuncClamp;
  fIdentFuncTable[8518] := FuncClip;
  fIdentFuncTable[4148] := FuncColor;
  fIdentFuncTable[9543] := FuncColor0;
  fIdentFuncTable[9544] := FuncColor1;
  fIdentFuncTable[9545] := FuncColor2;
  fIdentFuncTable[9546] := FuncColor3;
  fIdentFuncTable[9547] := FuncColor4;
  fIdentFuncTable[9548] := FuncColor5;
  fIdentFuncTable[9549] := FuncColor6;
  fIdentFuncTable[9550] := FuncColor7;
  fIdentFuncTable[9551] := FuncColor8;
  fIdentFuncTable[9552] := FuncColor9;
  fIdentFuncTable[14373] := FuncColumn95major;
  fIdentFuncTable[6598] := FuncConst;
  fIdentFuncTable[12948] := FuncConsumestructuredbuffer;
  fIdentFuncTable[8700] := FuncContinue;
  fIdentFuncTable[2069] := FuncCos;
  fIdentFuncTable[12318] := FuncCosh;
  fIdentFuncTable[680] := FuncCountbits;
  fIdentFuncTable[9401] := FuncCross;
  fIdentFuncTable[5092] := FuncD3dcolortoubyte4;
  fIdentFuncTable[2856] := FuncDdx;
  fIdentFuncTable[9213] := FuncDdx95coarse;
  fIdentFuncTable[8775] := FuncDdx95fine;
  fIdentFuncTable[2857] := FuncDdy;
  fIdentFuncTable[13035] := FuncDdy95coarse;
  fIdentFuncTable[14041] := FuncDdy95fine;
  fIdentFuncTable[9925] := FuncDefault;
  fIdentFuncTable[4673] := FuncDegrees;
  fIdentFuncTable[7583] := FuncDepth;
  fIdentFuncTable[10069] := FuncDepth0;
  fIdentFuncTable[10070] := FuncDepth1;
  fIdentFuncTable[10071] := FuncDepth2;
  fIdentFuncTable[10072] := FuncDepth3;
  fIdentFuncTable[10073] := FuncDepth4;
  fIdentFuncTable[10074] := FuncDepth5;
  fIdentFuncTable[10075] := FuncDepth6;
  fIdentFuncTable[10076] := FuncDepth7;
  fIdentFuncTable[10077] := FuncDepth8;
  fIdentFuncTable[10078] := FuncDepth9;
  fIdentFuncTable[5390] := FuncDeterminant;
  fIdentFuncTable[8510] := FuncDevicememorybarrier;
  fIdentFuncTable[9177] := FuncDevicememorybarrierwithgroupsync;
  fIdentFuncTable[4900] := FuncDiscard;
  fIdentFuncTable[2593] := FuncDistance;
  fIdentFuncTable[3511] := FuncDo;
  fIdentFuncTable[3226] := FuncDot;
  fIdentFuncTable[4574] := FuncDouble;
  fIdentFuncTable[8015] := FuncDouble1;
  fIdentFuncTable[4098] := FuncDouble1x1;
  fIdentFuncTable[4099] := FuncDouble1x2;
  fIdentFuncTable[4100] := FuncDouble1x3;
  fIdentFuncTable[4101] := FuncDouble1x4;
  fIdentFuncTable[8016] := FuncDouble2;
  fIdentFuncTable[5254] := FuncDouble2x1;
  fIdentFuncTable[5255] := FuncDouble2x2;
  fIdentFuncTable[5256] := FuncDouble2x3;
  fIdentFuncTable[5257] := FuncDouble2x4;
  fIdentFuncTable[8017] := FuncDouble3;
  fIdentFuncTable[6410] := FuncDouble3x1;
  fIdentFuncTable[6411] := FuncDouble3x2;
  fIdentFuncTable[6412] := FuncDouble3x3;
  fIdentFuncTable[6413] := FuncDouble3x4;
  fIdentFuncTable[8018] := FuncDouble4;
  fIdentFuncTable[7566] := FuncDouble4x1;
  fIdentFuncTable[7567] := FuncDouble4x2;
  fIdentFuncTable[7568] := FuncDouble4x3;
  fIdentFuncTable[7569] := FuncDouble4x4;
  fIdentFuncTable[3362] := FuncDst;
  fIdentFuncTable[1710] := FuncDword;
  fIdentFuncTable[13850] := FuncDword1;
  fIdentFuncTable[5161] := FuncDword1x1;
  fIdentFuncTable[5162] := FuncDword1x2;
  fIdentFuncTable[5163] := FuncDword1x3;
  fIdentFuncTable[5164] := FuncDword1x4;
  fIdentFuncTable[13851] := FuncDword2;
  fIdentFuncTable[6317] := FuncDword2x1;
  fIdentFuncTable[6318] := FuncDword2x2;
  fIdentFuncTable[6319] := FuncDword2x3;
  fIdentFuncTable[6320] := FuncDword2x4;
  fIdentFuncTable[13852] := FuncDword3;
  fIdentFuncTable[7473] := FuncDword3x1;
  fIdentFuncTable[7474] := FuncDword3x2;
  fIdentFuncTable[7475] := FuncDword3x3;
  fIdentFuncTable[7476] := FuncDword3x4;
  fIdentFuncTable[13853] := FuncDword4;
  fIdentFuncTable[8629] := FuncDword4x1;
  fIdentFuncTable[8630] := FuncDword4x2;
  fIdentFuncTable[8631] := FuncDword4x3;
  fIdentFuncTable[8632] := FuncDword4x4;
  fIdentFuncTable[257] := FuncElse;
  fIdentFuncTable[6792] := FuncErrorf;
  fIdentFuncTable[10908] := FuncEvaluateattributeatcentroid;
  fIdentFuncTable[1365] := FuncEvaluateattributeatsample;
  fIdentFuncTable[8504] := FuncEvaluateattributesnapped;
  fIdentFuncTable[4684] := FuncExp;
  fIdentFuncTable[13976] := FuncExp2;
  fIdentFuncTable[11516] := FuncExtern;
  fIdentFuncTable[8170] := FuncF16to32;
  fIdentFuncTable[6905] := FuncF32to16;
  fIdentFuncTable[2264] := FuncFaceforward;
  fIdentFuncTable[4169] := FuncFalse;
  fIdentFuncTable[12419] := FuncFirstbithigh;
  fIdentFuncTable[11879] := FuncFirstbitlow;
  fIdentFuncTable[5644] := FuncFlatten;
  fIdentFuncTable[3394] := FuncFloat;
  fIdentFuncTable[12974] := FuncFloat1;
  fIdentFuncTable[8808] := FuncFloat1x1;
  fIdentFuncTable[8809] := FuncFloat1x2;
  fIdentFuncTable[8810] := FuncFloat1x3;
  fIdentFuncTable[8811] := FuncFloat1x4;
  fIdentFuncTable[12975] := FuncFloat2;
  fIdentFuncTable[9964] := FuncFloat2x1;
  fIdentFuncTable[9965] := FuncFloat2x2;
  fIdentFuncTable[9966] := FuncFloat2x3;
  fIdentFuncTable[9967] := FuncFloat2x4;
  fIdentFuncTable[12976] := FuncFloat3;
  fIdentFuncTable[11120] := FuncFloat3x1;
  fIdentFuncTable[11121] := FuncFloat3x2;
  fIdentFuncTable[11122] := FuncFloat3x3;
  fIdentFuncTable[11123] := FuncFloat3x4;
  fIdentFuncTable[12977] := FuncFloat4;
  fIdentFuncTable[12276] := FuncFloat4x1;
  fIdentFuncTable[12277] := FuncFloat4x2;
  fIdentFuncTable[12278] := FuncFloat4x3;
  fIdentFuncTable[12279] := FuncFloat4x4;
  fIdentFuncTable[3868] := FuncFloor;
  fIdentFuncTable[5451] := FuncFma;
  fIdentFuncTable[11514] := FuncFmod;
  fIdentFuncTable[5525] := FuncFog;
  fIdentFuncTable[5536] := FuncFor;
  fIdentFuncTable[2284] := FuncFrac;
  fIdentFuncTable[10441] := FuncFrexp;
  fIdentFuncTable[5501] := FuncFwidth;
  fIdentFuncTable[2598] := FuncGetrendertargetsamplecount;
  fIdentFuncTable[2636] := FuncGetrendertargetsampleposition;
  fIdentFuncTable[7270] := FuncGroupmemorybarrier;
  fIdentFuncTable[6903] := FuncGroupmemorybarrierwithgroupsync;
  fIdentFuncTable[9020] := FuncGroupshared;
  fIdentFuncTable[3485] := FuncHalf;
  fIdentFuncTable[2275] := FuncHalf1;
  fIdentFuncTable[4502] := FuncHalf1x1;
  fIdentFuncTable[4503] := FuncHalf1x2;
  fIdentFuncTable[4504] := FuncHalf1x3;
  fIdentFuncTable[4505] := FuncHalf1x4;
  fIdentFuncTable[2276] := FuncHalf2;
  fIdentFuncTable[5658] := FuncHalf2x1;
  fIdentFuncTable[5659] := FuncHalf2x2;
  fIdentFuncTable[5660] := FuncHalf2x3;
  fIdentFuncTable[5661] := FuncHalf2x4;
  fIdentFuncTable[2277] := FuncHalf3;
  fIdentFuncTable[6814] := FuncHalf3x1;
  fIdentFuncTable[6815] := FuncHalf3x2;
  fIdentFuncTable[6816] := FuncHalf3x3;
  fIdentFuncTable[6817] := FuncHalf3x4;
  fIdentFuncTable[2278] := FuncHalf4;
  fIdentFuncTable[7970] := FuncHalf4x1;
  fIdentFuncTable[7971] := FuncHalf4x2;
  fIdentFuncTable[7972] := FuncHalf4x3;
  fIdentFuncTable[7973] := FuncHalf4x4;
  fIdentFuncTable[3672] := FuncIf;
  fIdentFuncTable[11246] := FuncIfall;
  fIdentFuncTable[11327] := FuncIfany;
  fIdentFuncTable[3680] := FuncIn;
  fIdentFuncTable[5754] := FuncInline;
  fIdentFuncTable[7917] := FuncInout;
  fIdentFuncTable[8589] := FuncInputpatch;
  fIdentFuncTable[8972] := FuncInt;
  fIdentFuncTable[14437] := FuncInt1;
  fIdentFuncTable[8677] := FuncInt1x1;
  fIdentFuncTable[8678] := FuncInt1x2;
  fIdentFuncTable[8679] := FuncInt1x3;
  fIdentFuncTable[8680] := FuncInt1x4;
  fIdentFuncTable[14438] := FuncInt2;
  fIdentFuncTable[9833] := FuncInt2x1;
  fIdentFuncTable[9834] := FuncInt2x2;
  fIdentFuncTable[9835] := FuncInt2x3;
  fIdentFuncTable[9836] := FuncInt2x4;
  fIdentFuncTable[14439] := FuncInt3;
  fIdentFuncTable[10989] := FuncInt3x1;
  fIdentFuncTable[10990] := FuncInt3x2;
  fIdentFuncTable[10991] := FuncInt3x3;
  fIdentFuncTable[10992] := FuncInt3x4;
  fIdentFuncTable[14440] := FuncInt4;
  fIdentFuncTable[12145] := FuncInt4x1;
  fIdentFuncTable[12146] := FuncInt4x2;
  fIdentFuncTable[12147] := FuncInt4x3;
  fIdentFuncTable[12148] := FuncInt4x4;
  fIdentFuncTable[14376] := FuncInterlockedadd;
  fIdentFuncTable[183] := FuncInterlockedand;
  fIdentFuncTable[7911] := FuncInterlockedcompareexchange;
  fIdentFuncTable[1143] := FuncInterlockedcomparestore;
  fIdentFuncTable[1596] := FuncInterlockedexchange;
  fIdentFuncTable[13633] := FuncInterlockedmax;
  fIdentFuncTable[13895] := FuncInterlockedmin;
  fIdentFuncTable[3801] := FuncInterlockedor;
  fIdentFuncTable[12286] := FuncInterlockedxor;
  fIdentFuncTable[820] := FuncIsfinite;
  fIdentFuncTable[8320] := FuncIsinf;
  fIdentFuncTable[13666] := FuncIsnan;
  fIdentFuncTable[14468] := FuncIsolate;
  fIdentFuncTable[8239] := FuncLdexp;
  fIdentFuncTable[2624] := FuncLength;
  fIdentFuncTable[5676] := FuncLerp;
  fIdentFuncTable[10153] := FuncLine;
  fIdentFuncTable[5978] := FuncLineadj;
  fIdentFuncTable[5328] := FuncLinestream;
  fIdentFuncTable[12270] := FuncLit;
  fIdentFuncTable[12461] := FuncLog;
  fIdentFuncTable[4427] := FuncLog10;
  fIdentFuncTable[2267] := FuncLog2;
  fIdentFuncTable[2601] := FuncLoop;
  fIdentFuncTable[13138] := FuncMad;
  fIdentFuncTable[11811] := FuncMatrix;
  fIdentFuncTable[13158] := FuncMax;
  fIdentFuncTable[134] := FuncMaxexports;
  fIdentFuncTable[11040] := FuncMaxinstructioncount;
  fIdentFuncTable[6982] := FuncMaxtempreg;
  fIdentFuncTable[13420] := FuncMin;
  fIdentFuncTable[3497] := FuncMin10float;
  fIdentFuncTable[196] := FuncMin10float1;
  fIdentFuncTable[8667] := FuncMin10float1x1;
  fIdentFuncTable[8668] := FuncMin10float1x2;
  fIdentFuncTable[8669] := FuncMin10float1x3;
  fIdentFuncTable[8670] := FuncMin10float1x4;
  fIdentFuncTable[197] := FuncMin10float2;
  fIdentFuncTable[9823] := FuncMin10float2x1;
  fIdentFuncTable[9824] := FuncMin10float2x2;
  fIdentFuncTable[9825] := FuncMin10float2x3;
  fIdentFuncTable[9826] := FuncMin10float2x4;
  fIdentFuncTable[198] := FuncMin10float3;
  fIdentFuncTable[10979] := FuncMin10float3x1;
  fIdentFuncTable[10980] := FuncMin10float3x2;
  fIdentFuncTable[10981] := FuncMin10float3x3;
  fIdentFuncTable[10982] := FuncMin10float3x4;
  fIdentFuncTable[199] := FuncMin10float4;
  fIdentFuncTable[12135] := FuncMin10float4x1;
  fIdentFuncTable[12136] := FuncMin10float4x2;
  fIdentFuncTable[12137] := FuncMin10float4x3;
  fIdentFuncTable[12138] := FuncMin10float4x4;
  fIdentFuncTable[5494] := FuncMin12int;
  fIdentFuncTable[4782] := FuncMin12int1;
  fIdentFuncTable[4455] := FuncMin12int1x1;
  fIdentFuncTable[4456] := FuncMin12int1x2;
  fIdentFuncTable[4457] := FuncMin12int1x3;
  fIdentFuncTable[4458] := FuncMin12int1x4;
  fIdentFuncTable[4783] := FuncMin12int2;
  fIdentFuncTable[5611] := FuncMin12int2x1;
  fIdentFuncTable[5612] := FuncMin12int2x2;
  fIdentFuncTable[5613] := FuncMin12int2x3;
  fIdentFuncTable[5614] := FuncMin12int2x4;
  fIdentFuncTable[4784] := FuncMin12int3;
  fIdentFuncTable[6767] := FuncMin12int3x1;
  fIdentFuncTable[6768] := FuncMin12int3x2;
  fIdentFuncTable[6769] := FuncMin12int3x3;
  fIdentFuncTable[6770] := FuncMin12int3x4;
  fIdentFuncTable[4785] := FuncMin12int4;
  fIdentFuncTable[7923] := FuncMin12int4x1;
  fIdentFuncTable[7924] := FuncMin12int4x2;
  fIdentFuncTable[7925] := FuncMin12int4x3;
  fIdentFuncTable[7926] := FuncMin12int4x4;
  fIdentFuncTable[6027] := FuncMin16float;
  fIdentFuncTable[12071] := FuncMin16float1;
  fIdentFuncTable[12392] := FuncMin16float1x1;
  fIdentFuncTable[12393] := FuncMin16float1x2;
  fIdentFuncTable[12394] := FuncMin16float1x3;
  fIdentFuncTable[12395] := FuncMin16float1x4;
  fIdentFuncTable[12072] := FuncMin16float2;
  fIdentFuncTable[13548] := FuncMin16float2x1;
  fIdentFuncTable[13549] := FuncMin16float2x2;
  fIdentFuncTable[13550] := FuncMin16float2x3;
  fIdentFuncTable[13551] := FuncMin16float2x4;
  fIdentFuncTable[12073] := FuncMin16float3;
  fIdentFuncTable[171] := FuncMin16float3x1;
  fIdentFuncTable[172] := FuncMin16float3x2;
  fIdentFuncTable[173] := FuncMin16float3x3;
  fIdentFuncTable[174] := FuncMin16float3x4;
  fIdentFuncTable[12074] := FuncMin16float4;
  fIdentFuncTable[1327] := FuncMin16float4x1;
  fIdentFuncTable[1328] := FuncMin16float4x2;
  fIdentFuncTable[1329] := FuncMin16float4x3;
  fIdentFuncTable[1330] := FuncMin16float4x4;
  fIdentFuncTable[2847] := FuncMin16int;
  fIdentFuncTable[1982] := FuncMin16int1;
  fIdentFuncTable[7774] := FuncMin16int1x1;
  fIdentFuncTable[7775] := FuncMin16int1x2;
  fIdentFuncTable[7776] := FuncMin16int1x3;
  fIdentFuncTable[7777] := FuncMin16int1x4;
  fIdentFuncTable[1983] := FuncMin16int2;
  fIdentFuncTable[8930] := FuncMin16int2x1;
  fIdentFuncTable[8931] := FuncMin16int2x2;
  fIdentFuncTable[8932] := FuncMin16int2x3;
  fIdentFuncTable[8933] := FuncMin16int2x4;
  fIdentFuncTable[1984] := FuncMin16int3;
  fIdentFuncTable[10086] := FuncMin16int3x1;
  fIdentFuncTable[10087] := FuncMin16int3x2;
  fIdentFuncTable[10088] := FuncMin16int3x3;
  fIdentFuncTable[10089] := FuncMin16int3x4;
  fIdentFuncTable[1985] := FuncMin16int4;
  fIdentFuncTable[11242] := FuncMin16int4x1;
  fIdentFuncTable[11243] := FuncMin16int4x2;
  fIdentFuncTable[11244] := FuncMin16int4x3;
  fIdentFuncTable[11245] := FuncMin16int4x4;
  fIdentFuncTable[2657] := FuncMin16uint;
  fIdentFuncTable[7835] := FuncMin16uint1;
  fIdentFuncTable[12335] := FuncMin16uint1x1;
  fIdentFuncTable[12336] := FuncMin16uint1x2;
  fIdentFuncTable[12337] := FuncMin16uint1x3;
  fIdentFuncTable[12338] := FuncMin16uint1x4;
  fIdentFuncTable[7836] := FuncMin16uint2;
  fIdentFuncTable[13491] := FuncMin16uint2x1;
  fIdentFuncTable[13492] := FuncMin16uint2x2;
  fIdentFuncTable[13493] := FuncMin16uint2x3;
  fIdentFuncTable[13494] := FuncMin16uint2x4;
  fIdentFuncTable[7837] := FuncMin16uint3;
  fIdentFuncTable[114] := FuncMin16uint3x1;
  fIdentFuncTable[115] := FuncMin16uint3x2;
  fIdentFuncTable[116] := FuncMin16uint3x3;
  fIdentFuncTable[117] := FuncMin16uint3x4;
  fIdentFuncTable[7838] := FuncMin16uint4;
  fIdentFuncTable[1270] := FuncMin16uint4x1;
  fIdentFuncTable[1271] := FuncMin16uint4x2;
  fIdentFuncTable[1272] := FuncMin16uint4x3;
  fIdentFuncTable[1273] := FuncMin16uint4x4;
  fIdentFuncTable[12455] := FuncModf;
  fIdentFuncTable[10415] := FuncMsad4;
  fIdentFuncTable[13826] := FuncMul;
  fIdentFuncTable[912] := FuncNamespace;
  fIdentFuncTable[5755] := FuncNoexpressionoptimizations;
  fIdentFuncTable[9114] := FuncNointerpolation;
  fIdentFuncTable[7636] := FuncNoise;
  fIdentFuncTable[9803] := FuncNormal;
  fIdentFuncTable[9184] := FuncNormal0;
  fIdentFuncTable[9185] := FuncNormal1;
  fIdentFuncTable[9186] := FuncNormal2;
  fIdentFuncTable[9187] := FuncNormal3;
  fIdentFuncTable[9188] := FuncNormal4;
  fIdentFuncTable[9189] := FuncNormal5;
  fIdentFuncTable[9190] := FuncNormal6;
  fIdentFuncTable[9191] := FuncNormal7;
  fIdentFuncTable[9192] := FuncNormal8;
  fIdentFuncTable[9193] := FuncNormal9;
  fIdentFuncTable[10568] := FuncNormalize;
  fIdentFuncTable[12968] := FuncNumthreads;
  fIdentFuncTable[1613] := FuncOut;
  fIdentFuncTable[13897] := FuncOutputpatch;
  fIdentFuncTable[1380] := FuncPackoffset;
  fIdentFuncTable[6081] := FuncPoint;
  fIdentFuncTable[4087] := FuncPointstream;
  fIdentFuncTable[7910] := FuncPosition;
  fIdentFuncTable[1207] := FuncPosition0;
  fIdentFuncTable[1208] := FuncPosition1;
  fIdentFuncTable[1209] := FuncPosition2;
  fIdentFuncTable[1210] := FuncPosition3;
  fIdentFuncTable[1211] := FuncPosition4;
  fIdentFuncTable[1212] := FuncPosition5;
  fIdentFuncTable[1213] := FuncPosition6;
  fIdentFuncTable[1214] := FuncPosition7;
  fIdentFuncTable[1215] := FuncPosition8;
  fIdentFuncTable[1216] := FuncPosition9;
  fIdentFuncTable[1275] := FuncPositiont;
  fIdentFuncTable[2568] := FuncPow;
  fIdentFuncTable[7929] := FuncPrecise;
  fIdentFuncTable[646] := FuncPredicate;
  fIdentFuncTable[1631] := FuncPredicateblock;
  fIdentFuncTable[554] := FuncPrintf;
  fIdentFuncTable[1120] := FuncProcess2dquadtessfactorsavg;
  fIdentFuncTable[14295] := FuncProcess2dquadtessfactorsmax;
  fIdentFuncTable[24] := FuncProcess2dquadtessfactorsmin;
  fIdentFuncTable[9620] := FuncProcessisolinetessfactors;
  fIdentFuncTable[14394] := FuncProcessquadtessfactorsavg;
  fIdentFuncTable[13036] := FuncProcessquadtessfactorsmax;
  fIdentFuncTable[13298] := FuncProcessquadtessfactorsmin;
  fIdentFuncTable[6998] := FuncProcesstritessfactorsavg;
  fIdentFuncTable[5640] := FuncProcesstritessfactorsmax;
  fIdentFuncTable[5902] := FuncProcesstritessfactorsmin;
  fIdentFuncTable[3827] := FuncPsize;
  fIdentFuncTable[13162] := FuncPsize0;
  fIdentFuncTable[13163] := FuncPsize1;
  fIdentFuncTable[13164] := FuncPsize2;
  fIdentFuncTable[13165] := FuncPsize3;
  fIdentFuncTable[13166] := FuncPsize4;
  fIdentFuncTable[13167] := FuncPsize5;
  fIdentFuncTable[13168] := FuncPsize6;
  fIdentFuncTable[13169] := FuncPsize7;
  fIdentFuncTable[13170] := FuncPsize8;
  fIdentFuncTable[13171] := FuncPsize9;
  fIdentFuncTable[8753] := FuncRadians;
  fIdentFuncTable[4465] := FuncRcp;
  fIdentFuncTable[3324] := FuncReducetempregusage;
  fIdentFuncTable[5250] := FuncReflect;
  fIdentFuncTable[3922] := FuncRefract;
  fIdentFuncTable[11975] := FuncRegister;
  fIdentFuncTable[5777] := FuncRemoveunusedinputs;
  fIdentFuncTable[10039] := FuncReturn;
  fIdentFuncTable[1884] := FuncReversebits;
  fIdentFuncTable[4004] := FuncRound;
  fIdentFuncTable[974] := FuncRow95major;
  fIdentFuncTable[11418] := FuncRsqrt;
  fIdentFuncTable[8922] := FuncRwbuffer;
  fIdentFuncTable[4489] := FuncRwbyteaddressbuffer;
  fIdentFuncTable[8322] := FuncRwstructuredbuffer;
  fIdentFuncTable[11397] := FuncRwtexture1d;
  fIdentFuncTable[10198] := FuncRwtexture1darray;
  fIdentFuncTable[11431] := FuncRwtexture2d;
  fIdentFuncTable[313] := FuncRwtexture2darray;
  fIdentFuncTable[11465] := FuncRwtexture3d;
  fIdentFuncTable[13111] := FuncSample;
  fIdentFuncTable[3978] := FuncSampler;
  fIdentFuncTable[8197] := FuncSamplercomparisonstate;
  fIdentFuncTable[8362] := FuncSamplerstate;
  fIdentFuncTable[10903] := FuncSampreg;
  fIdentFuncTable[11765] := FuncSaturate;
  fIdentFuncTable[3692] := FuncShared;
  fIdentFuncTable[8925] := FuncSign;
  fIdentFuncTable[5823] := FuncSin;
  fIdentFuncTable[2837] := FuncSincos;
  fIdentFuncTable[9157] := FuncSinh;
  fIdentFuncTable[5867] := FuncSmoothstep;
  fIdentFuncTable[808] := FuncSnorm;
  fIdentFuncTable[4020] := FuncSqrt;
  fIdentFuncTable[12272] := FuncStatic;
  fIdentFuncTable[7042] := FuncStep;
  fIdentFuncTable[12891] := FuncStruct;
  fIdentFuncTable[14476] := FuncStructuredbuffer;
  fIdentFuncTable[10778] := FuncSv95clipdistance;
  fIdentFuncTable[9568] := FuncSv95clipdistance0;
  fIdentFuncTable[9569] := FuncSv95clipdistance1;
  fIdentFuncTable[9570] := FuncSv95clipdistance2;
  fIdentFuncTable[9571] := FuncSv95clipdistance3;
  fIdentFuncTable[9572] := FuncSv95clipdistance4;
  fIdentFuncTable[9573] := FuncSv95clipdistance5;
  fIdentFuncTable[9574] := FuncSv95clipdistance6;
  fIdentFuncTable[9575] := FuncSv95clipdistance7;
  fIdentFuncTable[9576] := FuncSv95clipdistance8;
  fIdentFuncTable[9577] := FuncSv95clipdistance9;
  fIdentFuncTable[14416] := FuncSv95coverage;
  fIdentFuncTable[12354] := FuncSv95culldistance;
  fIdentFuncTable[10200] := FuncSv95culldistance0;
  fIdentFuncTable[10201] := FuncSv95culldistance1;
  fIdentFuncTable[10202] := FuncSv95culldistance2;
  fIdentFuncTable[10203] := FuncSv95culldistance3;
  fIdentFuncTable[10204] := FuncSv95culldistance4;
  fIdentFuncTable[10205] := FuncSv95culldistance5;
  fIdentFuncTable[10206] := FuncSv95culldistance6;
  fIdentFuncTable[10207] := FuncSv95culldistance7;
  fIdentFuncTable[10208] := FuncSv95culldistance8;
  fIdentFuncTable[10209] := FuncSv95culldistance9;
  fIdentFuncTable[6020] := FuncSv95depth;
  fIdentFuncTable[2008] := FuncSv95dispatchthreadid;
  fIdentFuncTable[7055] := FuncSv95domainlocation;
  fIdentFuncTable[12937] := FuncSv95groupid;
  fIdentFuncTable[2092] := FuncSv95groupindex;
  fIdentFuncTable[8233] := FuncSv95groupthreadid;
  fIdentFuncTable[5961] := FuncSv95gsinstanceid;
  fIdentFuncTable[9255] := FuncSv95insidetessfactor;
  fIdentFuncTable[5686] := FuncSv95instanceid;
  fIdentFuncTable[11477] := FuncSv95isfrontface;
  fIdentFuncTable[9348] := FuncSv95outputcontrolpointid;
  fIdentFuncTable[624] := FuncSv95position;
  fIdentFuncTable[9989] := FuncSv95primitiveid;
  fIdentFuncTable[5790] := FuncSv95rendertargetarrayindex;
  fIdentFuncTable[8125] := FuncSv95sampleindex;
  fIdentFuncTable[351] := FuncSv95target;
  fIdentFuncTable[11982] := FuncSv95target0;
  fIdentFuncTable[11983] := FuncSv95target1;
  fIdentFuncTable[11984] := FuncSv95target2;
  fIdentFuncTable[11985] := FuncSv95target3;
  fIdentFuncTable[11986] := FuncSv95target4;
  fIdentFuncTable[11987] := FuncSv95target5;
  fIdentFuncTable[11988] := FuncSv95target6;
  fIdentFuncTable[11989] := FuncSv95target7;
  fIdentFuncTable[9940] := FuncSv95tessfactor;
  fIdentFuncTable[3542] := FuncSv95vertexid;
  fIdentFuncTable[8185] := FuncSv95viewportarrayindex;
  fIdentFuncTable[4679] := FuncSwitch;
  fIdentFuncTable[6707] := FuncTan;
  fIdentFuncTable[9731] := FuncTangent;
  fIdentFuncTable[6469] := FuncTangent0;
  fIdentFuncTable[6470] := FuncTangent1;
  fIdentFuncTable[6471] := FuncTangent2;
  fIdentFuncTable[6472] := FuncTangent3;
  fIdentFuncTable[6473] := FuncTangent4;
  fIdentFuncTable[6474] := FuncTangent5;
  fIdentFuncTable[6475] := FuncTangent6;
  fIdentFuncTable[6476] := FuncTangent7;
  fIdentFuncTable[6477] := FuncTangent8;
  fIdentFuncTable[6478] := FuncTangent9;
  fIdentFuncTable[10147] := FuncTanh;
  fIdentFuncTable[707] := FuncTbuffer;
  fIdentFuncTable[11541] := FuncTessfactor;
  fIdentFuncTable[4224] := FuncTessfactor0;
  fIdentFuncTable[4225] := FuncTessfactor1;
  fIdentFuncTable[4226] := FuncTessfactor2;
  fIdentFuncTable[4227] := FuncTessfactor3;
  fIdentFuncTable[4228] := FuncTessfactor4;
  fIdentFuncTable[4229] := FuncTessfactor5;
  fIdentFuncTable[4230] := FuncTessfactor6;
  fIdentFuncTable[4231] := FuncTessfactor7;
  fIdentFuncTable[4232] := FuncTessfactor8;
  fIdentFuncTable[4233] := FuncTessfactor9;
  fIdentFuncTable[3349] := FuncTex1d;
  fIdentFuncTable[4882] := FuncTex1dbias;
  fIdentFuncTable[8329] := FuncTex1dgrad;
  fIdentFuncTable[14175] := FuncTex1dlod;
  fIdentFuncTable[13755] := FuncTex1dproj;
  fIdentFuncTable[3383] := FuncTex2d;
  fIdentFuncTable[10148] := FuncTex2dbias;
  fIdentFuncTable[13595] := FuncTex2dgrad;
  fIdentFuncTable[13475] := FuncTex2dlod;
  fIdentFuncTable[4488] := FuncTex2dproj;
  fIdentFuncTable[3417] := FuncTex3d;
  fIdentFuncTable[881] := FuncTex3dbias;
  fIdentFuncTable[4328] := FuncTex3dgrad;
  fIdentFuncTable[12775] := FuncTex3dlod;
  fIdentFuncTable[9754] := FuncTex3dproj;
  fIdentFuncTable[8165] := FuncTexcoord;
  fIdentFuncTable[11624] := FuncTexcoord0;
  fIdentFuncTable[11625] := FuncTexcoord1;
  fIdentFuncTable[11626] := FuncTexcoord2;
  fIdentFuncTable[11627] := FuncTexcoord3;
  fIdentFuncTable[11628] := FuncTexcoord4;
  fIdentFuncTable[11629] := FuncTexcoord5;
  fIdentFuncTable[11630] := FuncTexcoord6;
  fIdentFuncTable[11631] := FuncTexcoord7;
  fIdentFuncTable[11632] := FuncTexcoord8;
  fIdentFuncTable[11633] := FuncTexcoord9;
  fIdentFuncTable[916] := FuncTexcube;
  fIdentFuncTable[8337] := FuncTexcubebias;
  fIdentFuncTable[11784] := FuncTexcubegrad;
  fIdentFuncTable[5888] := FuncTexcubelod;
  fIdentFuncTable[2677] := FuncTexcubeproj;
  fIdentFuncTable[9219] := FuncTexture1d;
  fIdentFuncTable[7435] := FuncTexture1darray;
  fIdentFuncTable[9253] := FuncTexture2d;
  fIdentFuncTable[12083] := FuncTexture2darray;
  fIdentFuncTable[6603] := FuncTexture2dms;
  fIdentFuncTable[5076] := FuncTexture2dmsarray;
  fIdentFuncTable[9287] := FuncTexture3d;
  fIdentFuncTable[4341] := FuncTexturecube;
  fIdentFuncTable[14048] := FuncTexturecubearray;
  fIdentFuncTable[6032] := FuncTranspose;
  fIdentFuncTable[2605] := FuncTrianglestream;
  fIdentFuncTable[968] := FuncTrue;
  fIdentFuncTable[4251] := FuncTrunc;
  fIdentFuncTable[579] := FuncUint;
  fIdentFuncTable[5202] := FuncUint1;
  fIdentFuncTable[12758] := FuncUint1x1;
  fIdentFuncTable[12759] := FuncUint1x2;
  fIdentFuncTable[12760] := FuncUint1x3;
  fIdentFuncTable[12761] := FuncUint1x4;
  fIdentFuncTable[5203] := FuncUint2;
  fIdentFuncTable[13914] := FuncUint2x1;
  fIdentFuncTable[13915] := FuncUint2x2;
  fIdentFuncTable[13916] := FuncUint2x3;
  fIdentFuncTable[13917] := FuncUint2x4;
  fIdentFuncTable[5204] := FuncUint3;
  fIdentFuncTable[537] := FuncUint3x1;
  fIdentFuncTable[538] := FuncUint3x2;
  fIdentFuncTable[539] := FuncUint3x3;
  fIdentFuncTable[540] := FuncUint3x4;
  fIdentFuncTable[5205] := FuncUint4;
  fIdentFuncTable[1693] := FuncUint4x1;
  fIdentFuncTable[1694] := FuncUint4x2;
  fIdentFuncTable[1695] := FuncUint4x3;
  fIdentFuncTable[1696] := FuncUint4x4;
  fIdentFuncTable[14383] := FuncUniform;
  fIdentFuncTable[13941] := FuncUnorm;
  fIdentFuncTable[6452] := FuncUnroll;
  fIdentFuncTable[10232] := FuncUnsigned;
  fIdentFuncTable[12478] := FuncUnused;
  fIdentFuncTable[1133] := FuncVector;
  fIdentFuncTable[3034] := FuncVoid;
  fIdentFuncTable[12522] := FuncVolatile;
  fIdentFuncTable[2097] := FuncWhile;
end;

{$Q-}
function TSynHLSLSyn.HashKey(Str: PWideChar): Cardinal;
begin
  Result := 0;
  while IsIdentChar(Str^) do
  begin
    Result := Result * 34 + Ord(Str^);
    inc(Str);
  end;
  Result := Result mod 14533;
  fStringLen := Str - fToIdent;
end;
{$Q+}

function TSynHLSLSyn.FuncAbort(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncAbs(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncAcos(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncAll(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncAllmemorybarrier(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncAllmemorybarrierwithgroupsync(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncAny(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncAppendstructuredbuffer(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkObjs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncAsdouble(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncAsfloat(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncAsin(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncAsint(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncAsuint(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncAtan(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncAtan2(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncBinormal(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncBinormal0(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncBinormal1(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncBinormal2(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncBinormal3(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncBinormal4(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncBinormal5(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncBinormal6(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncBinormal7(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncBinormal8(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncBinormal9(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncBlendindices(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncBlendindices0(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncBlendindices1(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncBlendindices2(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncBlendindices3(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncBlendindices4(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncBlendindices5(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncBlendindices6(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncBlendindices7(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncBlendindices8(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncBlendindices9(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncBlendweight(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncBlendweight0(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncBlendweight1(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncBlendweight2(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncBlendweight3(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncBlendweight4(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncBlendweight5(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncBlendweight6(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncBlendweight7(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncBlendweight8(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncBlendweight9(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncBool(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncBool1(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncBool1x1(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncBool1x2(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncBool1x3(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncBool1x4(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncBool2(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncBool2x1(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncBool2x2(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncBool2x3(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncBool2x4(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncBool3(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncBool3x1(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncBool3x2(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncBool3x3(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncBool3x4(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncBool4(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncBool4x1(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncBool4x2(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncBool4x3(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncBool4x4(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncBranch(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkShaderAttr
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncBreak(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkControlFlow
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncBuffer(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkObjs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncByteaddressbuffer(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkObjs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncCall(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkShaderAttr
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncCase(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkControlFlow
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncCbuffer(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkKey
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncCeil(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncCentroid(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkStorageClass
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncCheckaccessfullymapped(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncClamp(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncClip(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncColor(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncColor0(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncColor1(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncColor2(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncColor3(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncColor4(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncColor5(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncColor6(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncColor7(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncColor8(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncColor9(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncColumn95major(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkTypeMod
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncConst(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkTypeMod
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncConsumestructuredbuffer(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkObjs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncContinue(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkControlFlow
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncCos(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncCosh(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncCountbits(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncCross(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncD3dcolortoubyte4(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncDdx(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncDdx95coarse(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncDdx95fine(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncDdy(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncDdy95coarse(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncDdy95fine(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncDefault(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkKey
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncDegrees(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncDepth(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncDepth0(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncDepth1(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncDepth2(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncDepth3(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncDepth4(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncDepth5(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncDepth6(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncDepth7(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncDepth8(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncDepth9(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncDeterminant(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncDevicememorybarrier(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncDevicememorybarrierwithgroupsync(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncDiscard(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkControlFlow
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncDistance(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncDo(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkControlFlow
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncDot(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncDouble(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncDouble1(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncDouble1x1(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncDouble1x2(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncDouble1x3(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncDouble1x4(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncDouble2(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncDouble2x1(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncDouble2x2(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncDouble2x3(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncDouble2x4(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncDouble3(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncDouble3x1(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncDouble3x2(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncDouble3x3(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncDouble3x4(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncDouble4(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncDouble4x1(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncDouble4x2(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncDouble4x3(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncDouble4x4(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncDst(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncDword(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncDword1(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncDword1x1(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncDword1x2(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncDword1x3(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncDword1x4(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncDword2(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncDword2x1(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncDword2x2(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncDword2x3(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncDword2x4(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncDword3(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncDword3x1(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncDword3x2(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncDword3x3(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncDword3x4(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncDword4(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncDword4x1(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncDword4x2(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncDword4x3(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncDword4x4(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncElse(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkControlFlow
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncErrorf(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncEvaluateattributeatcentroid(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncEvaluateattributeatsample(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncEvaluateattributesnapped(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncExp(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncExp2(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncExtern(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkStorageClass
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncF16to32(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncF32to16(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncFaceforward(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncFalse(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkKey
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncFirstbithigh(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncFirstbitlow(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncFlatten(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkShaderAttr
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncFloat(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncFloat1(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncFloat1x1(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncFloat1x2(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncFloat1x3(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncFloat1x4(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncFloat2(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncFloat2x1(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncFloat2x2(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncFloat2x3(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncFloat2x4(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncFloat3(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncFloat3x1(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncFloat3x2(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncFloat3x3(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncFloat3x4(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncFloat4(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncFloat4x1(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncFloat4x2(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncFloat4x3(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncFloat4x4(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncFloor(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncFma(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncFmod(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncFog(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncFor(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkControlFlow
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncFrac(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncFrexp(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncFwidth(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncGetrendertargetsamplecount(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncGetrendertargetsampleposition(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncGroupmemorybarrier(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncGroupmemorybarrierwithgroupsync(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncGroupshared(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkStorageClass
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncHalf(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncHalf1(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncHalf1x1(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncHalf1x2(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncHalf1x3(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncHalf1x4(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncHalf2(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncHalf2x1(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncHalf2x2(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncHalf2x3(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncHalf2x4(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncHalf3(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncHalf3x1(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncHalf3x2(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncHalf3x3(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncHalf3x4(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncHalf4(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncHalf4x1(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncHalf4x2(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncHalf4x3(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncHalf4x4(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncIf(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkControlFlow
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncIfall(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkShaderAttr
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncIfany(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkShaderAttr
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncIn(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkVarUsage
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncInline(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkKey
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncInout(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkVarUsage
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncInputpatch(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkObjs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncInt(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncInt1(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncInt1x1(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncInt1x2(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncInt1x3(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncInt1x4(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncInt2(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncInt2x1(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncInt2x2(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncInt2x3(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncInt2x4(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncInt3(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncInt3x1(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncInt3x2(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncInt3x3(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncInt3x4(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncInt4(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncInt4x1(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncInt4x2(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncInt4x3(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncInt4x4(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncInterlockedadd(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncInterlockedand(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncInterlockedcompareexchange(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncInterlockedcomparestore(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncInterlockedexchange(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncInterlockedmax(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncInterlockedmin(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncInterlockedor(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncInterlockedxor(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncIsfinite(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncIsinf(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncIsnan(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncIsolate(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkShaderAttr
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncLdexp(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncLength(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncLerp(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncLine(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkKey
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncLineadj(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkKey
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncLinestream(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkObjs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncLit(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncLog(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncLog10(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncLog2(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncLoop(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkShaderAttr
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMad(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMatrix(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMax(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMaxexports(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkShaderAttr
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMaxinstructioncount(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkShaderAttr
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMaxtempreg(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkShaderAttr
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin10float(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin10float1(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin10float1x1(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin10float1x2(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin10float1x3(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin10float1x4(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin10float2(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin10float2x1(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin10float2x2(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin10float2x3(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin10float2x4(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin10float3(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin10float3x1(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin10float3x2(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin10float3x3(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin10float3x4(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin10float4(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin10float4x1(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin10float4x2(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin10float4x3(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin10float4x4(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin12int(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin12int1(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin12int1x1(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin12int1x2(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin12int1x3(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin12int1x4(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin12int2(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin12int2x1(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin12int2x2(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin12int2x3(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin12int2x4(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin12int3(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin12int3x1(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin12int3x2(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin12int3x3(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin12int3x4(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin12int4(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin12int4x1(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin12int4x2(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin12int4x3(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin12int4x4(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin16float(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin16float1(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin16float1x1(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin16float1x2(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin16float1x3(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin16float1x4(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin16float2(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin16float2x1(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin16float2x2(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin16float2x3(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin16float2x4(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin16float3(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin16float3x1(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin16float3x2(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin16float3x3(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin16float3x4(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin16float4(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin16float4x1(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin16float4x2(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin16float4x3(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin16float4x4(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin16int(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin16int1(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin16int1x1(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin16int1x2(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin16int1x3(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin16int1x4(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin16int2(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin16int2x1(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin16int2x2(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin16int2x3(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin16int2x4(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin16int3(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin16int3x1(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin16int3x2(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin16int3x3(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin16int3x4(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin16int4(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin16int4x1(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin16int4x2(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin16int4x3(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin16int4x4(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin16uint(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin16uint1(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin16uint1x1(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin16uint1x2(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin16uint1x3(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin16uint1x4(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin16uint2(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin16uint2x1(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin16uint2x2(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin16uint2x3(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin16uint2x4(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin16uint3(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin16uint3x1(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin16uint3x2(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin16uint3x3(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin16uint3x4(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin16uint4(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin16uint4x1(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin16uint4x2(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin16uint4x3(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMin16uint4x4(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncModf(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMsad4(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncMul(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncNamespace(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkKey
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncNoexpressionoptimizations(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkShaderAttr
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncNointerpolation(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkStorageClass
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncNoise(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncNormal(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncNormal0(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncNormal1(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncNormal2(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncNormal3(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncNormal4(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncNormal5(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncNormal6(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncNormal7(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncNormal8(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncNormal9(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncNormalize(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncNumthreads(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkShaderAttr
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncOut(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkVarUsage
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncOutputpatch(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkObjs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncPackoffset(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkMiscKey
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncPoint(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkKey
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncPointstream(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkObjs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncPosition(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncPosition0(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncPosition1(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncPosition2(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncPosition3(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncPosition4(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncPosition5(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncPosition6(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncPosition7(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncPosition8(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncPosition9(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncPositiont(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncPow(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncPrecise(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkStorageClass
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncPredicate(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkShaderAttr
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncPredicateblock(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkShaderAttr
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncPrintf(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncProcess2dquadtessfactorsavg(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncProcess2dquadtessfactorsmax(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncProcess2dquadtessfactorsmin(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncProcessisolinetessfactors(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncProcessquadtessfactorsavg(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncProcessquadtessfactorsmax(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncProcessquadtessfactorsmin(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncProcesstritessfactorsavg(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncProcesstritessfactorsmax(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncProcesstritessfactorsmin(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncPsize(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncPsize0(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncPsize1(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncPsize2(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncPsize3(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncPsize4(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncPsize5(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncPsize6(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncPsize7(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncPsize8(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncPsize9(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncRadians(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncRcp(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncReducetempregusage(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkShaderAttr
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncReflect(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncRefract(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncRegister(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkMiscKey
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncRemoveunusedinputs(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkShaderAttr
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncReturn(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkControlFlow
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncReversebits(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncRound(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncRow95major(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkTypeMod
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncRsqrt(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncRwbuffer(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkObjs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncRwbyteaddressbuffer(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkObjs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncRwstructuredbuffer(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkObjs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncRwtexture1d(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkObjs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncRwtexture1darray(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkObjs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncRwtexture2d(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkObjs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncRwtexture2darray(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkObjs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncRwtexture3d(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkObjs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncSample(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkStorageClass
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncSampler(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkObjs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncSamplercomparisonstate(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkObjs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncSamplerstate(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkObjs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncSampreg(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkShaderAttr
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncSaturate(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncShared(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkStorageClass
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncSign(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncSin(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncSincos(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncSinh(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncSmoothstep(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncSnorm(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncSqrt(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncStatic(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkStorageClass
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncStep(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncStruct(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkKey
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncStructuredbuffer(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkObjs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncSv95clipdistance(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncSv95clipdistance0(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncSv95clipdistance1(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncSv95clipdistance2(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncSv95clipdistance3(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncSv95clipdistance4(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncSv95clipdistance5(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncSv95clipdistance6(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncSv95clipdistance7(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncSv95clipdistance8(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncSv95clipdistance9(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncSv95coverage(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncSv95culldistance(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncSv95culldistance0(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncSv95culldistance1(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncSv95culldistance2(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncSv95culldistance3(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncSv95culldistance4(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncSv95culldistance5(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncSv95culldistance6(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncSv95culldistance7(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncSv95culldistance8(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncSv95culldistance9(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncSv95depth(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncSv95dispatchthreadid(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncSv95domainlocation(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncSv95groupid(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncSv95groupindex(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncSv95groupthreadid(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncSv95gsinstanceid(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncSv95insidetessfactor(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncSv95instanceid(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncSv95isfrontface(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncSv95outputcontrolpointid(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncSv95position(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncSv95primitiveid(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncSv95rendertargetarrayindex(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncSv95sampleindex(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncSv95target(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncSv95target0(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncSv95target1(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncSv95target2(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncSv95target3(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncSv95target4(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncSv95target5(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncSv95target6(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncSv95target7(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncSv95tessfactor(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncSv95vertexid(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncSv95viewportarrayindex(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncSwitch(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkControlFlow
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncTan(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncTangent(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncTangent0(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncTangent1(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncTangent2(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncTangent3(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncTangent4(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncTangent5(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncTangent6(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncTangent7(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncTangent8(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncTangent9(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncTanh(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncTbuffer(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkKey
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncTessfactor(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncTessfactor0(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncTessfactor1(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncTessfactor2(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncTessfactor3(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncTessfactor4(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncTessfactor5(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncTessfactor6(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncTessfactor7(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncTessfactor8(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncTessfactor9(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncTex1d(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncTex1dbias(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncTex1dgrad(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncTex1dlod(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncTex1dproj(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncTex2d(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncTex2dbias(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncTex2dgrad(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncTex2dlod(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncTex2dproj(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncTex3d(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncTex3dbias(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncTex3dgrad(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncTex3dlod(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncTex3dproj(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncTexcoord(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncTexcoord0(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncTexcoord1(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncTexcoord2(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncTexcoord3(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncTexcoord4(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncTexcoord5(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncTexcoord6(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncTexcoord7(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncTexcoord8(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncTexcoord9(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkSemantic
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncTexcube(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncTexcubebias(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncTexcubegrad(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncTexcubelod(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncTexcubeproj(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncTexture1d(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkObjs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncTexture1darray(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkObjs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncTexture2d(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkObjs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncTexture2darray(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkObjs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncTexture2dms(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkObjs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncTexture2dmsarray(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkObjs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncTexture3d(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkObjs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncTexturecube(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkObjs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncTexturecubearray(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkObjs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncTranspose(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncTrianglestream(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkObjs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncTrue(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkKey
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncTrunc(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkFuncs
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncUint(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncUint1(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncUint1x1(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncUint1x2(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncUint1x3(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncUint1x4(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncUint2(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncUint2x1(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncUint2x2(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncUint2x3(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncUint2x4(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncUint3(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncUint3x1(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncUint3x2(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncUint3x3(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncUint3x4(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncUint4(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncUint4x1(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncUint4x2(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncUint4x3(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncUint4x4(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncUniform(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkStorageClass
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncUnorm(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncUnroll(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkShaderAttr
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncUnsigned(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncUnused(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkShaderAttr
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncVector(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncVoid(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkBuiltInTypes
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncVolatile(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkStorageClass
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.FuncWhile(Index: Integer): TtkTokenKind;
begin
  if IsCurrentToken(KeyWords[Index]) then
    Result := tkControlFlow
  else
    Result := tkIdentifier;
end;

function TSynHLSLSyn.AltFunc(Index: Integer): TtkTokenKind;
begin
  Result := tkIdentifier;
end;

function TSynHLSLSyn.IdentKind(MayBe: PWideChar): TtkTokenKind;
var
  Key: Cardinal;
begin
  fToIdent := MayBe;
  Key := HashKey(MayBe);
  if Key <= High(fIdentFuncTable) then
    Result := fIdentFuncTable[Key](KeyIndices[Key])
  else
    Result := tkIdentifier;
end;

procedure TSynHLSLSyn.SpaceProc;
begin
  inc(Run);
  fTokenID := tkSpace;
  while (FLine[Run] <= #32) and not IsLineEnd(Run) do inc(Run);
end;

procedure TSynHLSLSyn.NullProc;
begin
  fTokenID := tkNull;
  inc(Run);
end;

procedure TSynHLSLSyn.CRProc;
begin
  fTokenID := tkSpace;
  inc(Run);
  if fLine[Run] = #10 then
    inc(Run);
end;

procedure TSynHLSLSyn.LFProc;
begin
  fTokenID := tkSpace;
  inc(Run);
end;

procedure TSynHLSLSyn.CStyleCommentOpenProc;
begin
  Inc(Run);
  if (fLine[Run] = '*') then
  begin
    Inc(Run, 1);
    fRange := rsCStyleComment;
    fTokenID := tkComment;
  end
  else if (fLine[Run] = '/') then
  begin
    Inc(Run, 1);
    fRange := rsUnknown;
    fTokenID := tkComment;
    while not IsLineEnd(Run) do
      Inc(Run);
  end
  else
    fTokenID := tkIdentifier;
end;

procedure TSynHLSLSyn.CStyleCommentProc;
begin
  case fLine[Run] of
     #0: NullProc;
    #10: LFProc;
    #13: CRProc;
  else
    begin
      fTokenID := tkComment;
      repeat
        if (fLine[Run] = '*') and
           (fLine[Run + 1] = '/') then
        begin
          Inc(Run, 2);
          fRange := rsUnKnown;
          Break;
        end;
        if not IsLineEnd(Run) then
          Inc(Run);
      until IsLineEnd(Run);
    end;
  end;
end;

procedure TSynHLSLSyn.StringOpenProc;
begin
  Inc(Run);
  fRange := rsString;
  StringProc;
  fTokenID := tkString;
end;

procedure TSynHLSLSyn.StringProc;
begin
  fTokenID := tkString;
  repeat
    if (fLine[Run] = '"') then
    begin
      Inc(Run, 1);
      fRange := rsUnKnown;
      Break;
    end;
    if not IsLineEnd(Run) then
      Inc(Run);
  until IsLineEnd(Run);
end;

constructor TSynHLSLSyn.Create(AOwner: TComponent);
begin
  inherited Create(AOwner);
  fCaseSensitive := False;

  fBuiltInTypesAttri := TSynHighLighterAttributes.Create(SYNS_AttrBuiltInTypes, SYNS_FriendlyAttrBuiltInTypes);
  fBuiltInTypesAttri.Style := [fsBold];
  fBuiltInTypesAttri.Foreground := clWebMediumBlue;
  AddAttribute(fBuiltInTypesAttri);

  fCommentAttri := TSynHighLighterAttributes.Create(SYNS_AttrComment, SYNS_FriendlyAttrComment);
  fCommentAttri.Style := [fsItalic];
  fCommentAttri.Foreground := clWebDarkOliveGreen;
  AddAttribute(fCommentAttri);

  fControlFlowAttri := TSynHighLighterAttributes.Create(SYNS_AttrControlFlow, SYNS_FriendlyAttrControlFlow);
  fControlFlowAttri.Style := [fsBold];
  AddAttribute(fControlFlowAttri);

  fFuncsAttri := TSynHighLighterAttributes.Create(SYNS_AttrFuncs, SYNS_FriendlyAttrFuncs);
  fFuncsAttri.Style := [fsBold];
  fFuncsAttri.Foreground := clWebSteelBlue;
  AddAttribute(fFuncsAttri);

  fIdentifierAttri := TSynHighLighterAttributes.Create(SYNS_AttrAsm, SYNS_FriendlyAttrAsm);
  AddAttribute(fIdentifierAttri);

  fKeyAttri := TSynHighLighterAttributes.Create(SYNS_AttrReservedWord, SYNS_FriendlyAttrReservedWord);
  fKeyAttri.Style := [fsBold];
  AddAttribute(fKeyAttri);

  fMiscKeyAttri := TSynHighLighterAttributes.Create(SYNS_AttrMiscKey, SYNS_FriendlyAttrMiscKey);
  fMiscKeyAttri.Style := [fsBold];
  fMiscKeyAttri.Foreground := clWebRoyalBlue;
  AddAttribute(fMiscKeyAttri);

  fObjsAttri := TSynHighLighterAttributes.Create(SYNS_AttrObjs, SYNS_FriendlyAttrObjs);
  fObjsAttri.Style := [fsBold];
  fObjsAttri.Foreground := clWebMediumBlue;
  AddAttribute(fObjsAttri);

  fSemanticAttri := TSynHighLighterAttributes.Create(SYNS_AttrSemantic, SYNS_FriendlyAttrSemantic);
  fSemanticAttri.Style := [fsBold];
  fSemanticAttri.Foreground := clWebGray;
  AddAttribute(fSemanticAttri);

  fShaderAttrAttri := TSynHighLighterAttributes.Create(SYNS_AttrShaderAttr, SYNS_FriendlyAttrShaderAttr);
  fShaderAttrAttri.Style := [fsItalic];
  fShaderAttrAttri.Foreground := clWebMidnightBlue;
  AddAttribute(fShaderAttrAttri);

  fSpaceAttri := TSynHighLighterAttributes.Create(SYNS_AttrSpace, SYNS_FriendlyAttrSpace);
  AddAttribute(fSpaceAttri);

  fStorageClassAttri := TSynHighLighterAttributes.Create(SYNS_AttrStorageClass, SYNS_FriendlyAttrStorageClass);
  fStorageClassAttri.Style := [fsBold];
  fStorageClassAttri.Foreground := clWebDarkSlateBlue;
  AddAttribute(fStorageClassAttri);

  fStringAttri := TSynHighLighterAttributes.Create(SYNS_AttrString, SYNS_FriendlyAttrString);
  fStringAttri.Foreground := clWebFirebrick;
  AddAttribute(fStringAttri);

  fTypeModAttri := TSynHighLighterAttributes.Create(SYNS_AttrTypeMod, SYNS_FriendlyAttrTypeMod);
  fTypeModAttri.Style := [fsBold];
  fTypeModAttri.Foreground := clWebSlateBlue;
  AddAttribute(fTypeModAttri);

  fVarUsageAttri := TSynHighLighterAttributes.Create(SYNS_AttrVarUsage, SYNS_FriendlyAttrVarUsage);
  fVarUsageAttri.Style := [fsBold];
  fVarUsageAttri.Foreground := clWebDarkSlategray;
  AddAttribute(fVarUsageAttri);

  SetAttributesOnChange(DefHighlightChange);
  InitIdent;
  fDefaultFilter := SYNS_FilterHLSL;
  fRange := rsUnknown;
end;

procedure TSynHLSLSyn.IdentProc;
begin
  fTokenID := IdentKind(fLine + Run);
  inc(Run, fStringLen);
  while IsIdentChar(fLine[Run]) do
    Inc(Run);
end;

procedure TSynHLSLSyn.UnknownProc;
begin
  inc(Run);
  fTokenID := tkUnknown;
end;

procedure TSynHLSLSyn.Next;
begin
  fTokenPos := Run;
  case fRange of
    rsCStyleComment: CStyleCommentProc;
  else
    case fLine[Run] of
      #0: NullProc;
      #10: LFProc;
      #13: CRProc;
      '/': CStyleCommentOpenProc;
      '"': StringOpenProc;
      #1..#9, #11, #12, #14..#32: SpaceProc;
      'A'..'Z', 'a'..'z', '_': IdentProc;
    else
      UnknownProc;
    end;
  end;
  inherited;
end;

function TSynHLSLSyn.GetDefaultAttribute(Index: Integer): TSynHighLighterAttributes;
begin
  case Index of
    SYN_ATTR_COMMENT: Result := fCommentAttri;
    SYN_ATTR_IDENTIFIER: Result := fIdentifierAttri;
    SYN_ATTR_KEYWORD: Result := fKeyAttri;
    SYN_ATTR_STRING: Result := fStringAttri;
    SYN_ATTR_WHITESPACE: Result := fSpaceAttri;
  else
    Result := nil;
  end;
end;

function TSynHLSLSyn.GetEol: Boolean;
begin
  Result := Run = fLineLen + 1;
end;

function TSynHLSLSyn.GetKeyWords(TokenKind: Integer): UnicodeString;
begin
  Result := 
    'abort,abs,acos,all,AllMemoryBarrier,AllMemoryBarrierWithGroupSync,any' +
    ',AppendStructuredBuffer,asdouble,asfloat,asin,asint,asuint,atan,atan2,' +
    'BINORMAL,BINORMAL0,BINORMAL1,BINORMAL2,BINORMAL3,BINORMAL4,BINORMAL5,B' +
    'INORMAL6,BINORMAL7,BINORMAL8,BINORMAL9,BLENDINDICES,BLENDINDICES0,BLEN' +
    'DINDICES1,BLENDINDICES2,BLENDINDICES3,BLENDINDICES4,BLENDINDICES5,BLEN' +
    'DINDICES6,BLENDINDICES7,BLENDINDICES8,BLENDINDICES9,BLENDWEIGHT,BLENDW' +
    'EIGHT0,BLENDWEIGHT1,BLENDWEIGHT2,BLENDWEIGHT3,BLENDWEIGHT4,BLENDWEIGHT' +
    '5,BLENDWEIGHT6,BLENDWEIGHT7,BLENDWEIGHT8,BLENDWEIGHT9,bool,bool1,bool1' +
    'x1,bool1x2,bool1x3,bool1x4,bool2,bool2x1,bool2x2,bool2x3,bool2x4,bool3' +
    ',bool3x1,bool3x2,bool3x3,bool3x4,bool4,bool4x1,bool4x2,bool4x3,bool4x4' +
    ',branch,break,Buffer,ByteAddressBuffer,call,case,cbuffer,ceil,centroid' +
    ',CheckAccessFullyMapped,clamp,clip,COLOR,COLOR0,COLOR1,COLOR2,COLOR3,C' +
    'OLOR4,COLOR5,COLOR6,COLOR7,COLOR8,COLOR9,column_major,const,ConsumeStr' +
    'ucturedBUffer,continue,cos,cosh,countbits,cross,D3DCOLORtoUBYTE4,ddx,d' +
    'dx_coarse,ddx_fine,ddy,ddy_coarse,ddy_fine,default,degrees,DEPTH,DEPTH' +
    '0,DEPTH1,DEPTH2,DEPTH3,DEPTH4,DEPTH5,DEPTH6,DEPTH7,DEPTH8,DEPTH9,deter' +
    'minant,DeviceMemoryBarrier,DeviceMemoryBarrierWithGroupSync,discard,di' +
    'stance,do,dot,double,double1,double1x1,double1x2,double1x3,double1x4,d' +
    'ouble2,double2x1,double2x2,double2x3,double2x4,double3,double3x1,doubl' +
    'e3x2,double3x3,double3x4,double4,double4x1,double4x2,double4x3,double4' +
    'x4,dst,dword,dword1,dword1x1,dword1x2,dword1x3,dword1x4,dword2,dword2x' +
    '1,dword2x2,dword2x3,dword2x4,dword3,dword3x1,dword3x2,dword3x3,dword3x' +
    '4,dword4,dword4x1,dword4x2,dword4x3,dword4x4,else,errorf,EvaluateAttri' +
    'buteAtCentroid,EvaluateAttributeAtSample,EvaluateAttributeSnapped,exp,' +
    'exp2,extern,f16to32,f32to16,faceforward,false,firstbithigh,firstbitlow' +
    ',flatten,float,float1,float1x1,float1x2,float1x3,float1x4,float2,float' +
    '2x1,float2x2,float2x3,float2x4,float3,float3x1,float3x2,float3x3,float' +
    '3x4,float4,float4x1,float4x2,float4x3,float4x4,floor,fma,fmod,FOG,for,' +
    'frac,frexp,fwidth,GetRenderTargetSampleCount,GetRenderTargetSamplePosi' +
    'tion,GroupMemoryBarrier,GroupMemoryBarrierWithGroupSync,groupshared,ha' +
    'lf,half1,half1x1,half1x2,half1x3,half1x4,half2,half2x1,half2x2,half2x3' +
    ',half2x4,half3,half3x1,half3x2,half3x3,half3x4,half4,half4x1,half4x2,h' +
    'alf4x3,half4x4,if,ifAll,ifAny,in,inline,inout,InputPatch,int,int1,int1' +
    'x1,int1x2,int1x3,int1x4,int2,int2x1,int2x2,int2x3,int2x4,int3,int3x1,i' +
    'nt3x2,int3x3,int3x4,int4,int4x1,int4x2,int4x3,int4x4,InterlockedAdd,In' +
    'terlockedAnd,InterlockedCompareExchange,InterlockedCompareStore,Interl' +
    'ockedExchange,InterlockedMax,InterlockedMin,InterlockedOr,InterlockedX' +
    'or,isfinite,isinf,isnan,isolate,ldexp,length,lerp,line,lineadj,LineStr' +
    'eam,lit,log,log10,log2,loop,mad,matrix,max,maxexports,maxInstructionCo' +
    'unt,maxtempreg,min,min10float,min10float1,min10float1x1,min10float1x2,' +
    'min10float1x3,min10float1x4,min10float2,min10float2x1,min10float2x2,mi' +
    'n10float2x3,min10float2x4,min10float3,min10float3x1,min10float3x2,min1' +
    '0float3x3,min10float3x4,min10float4,min10float4x1,min10float4x2,min10f' +
    'loat4x3,min10float4x4,min12int,min12int1,min12int1x1,min12int1x2,min12' +
    'int1x3,min12int1x4,min12int2,min12int2x1,min12int2x2,min12int2x3,min12' +
    'int2x4,min12int3,min12int3x1,min12int3x2,min12int3x3,min12int3x4,min12' +
    'int4,min12int4x1,min12int4x2,min12int4x3,min12int4x4,min16float,min16f' +
    'loat1,min16float1x1,min16float1x2,min16float1x3,min16float1x4,min16flo' +
    'at2,min16float2x1,min16float2x2,min16float2x3,min16float2x4,min16float' +
    '3,min16float3x1,min16float3x2,min16float3x3,min16float3x4,min16float4,' +
    'min16float4x1,min16float4x2,min16float4x3,min16float4x4,min16int,min16' +
    'int1,min16int1x1,min16int1x2,min16int1x3,min16int1x4,min16int2,min16in' +
    't2x1,min16int2x2,min16int2x3,min16int2x4,min16int3,min16int3x1,min16in' +
    't3x2,min16int3x3,min16int3x4,min16int4,min16int4x1,min16int4x2,min16in' +
    't4x3,min16int4x4,min16uint,min16uint1,min16uint1x1,min16uint1x2,min16u' +
    'int1x3,min16uint1x4,min16uint2,min16uint2x1,min16uint2x2,min16uint2x3,' +
    'min16uint2x4,min16uint3,min16uint3x1,min16uint3x2,min16uint3x3,min16ui' +
    'nt3x4,min16uint4,min16uint4x1,min16uint4x2,min16uint4x3,min16uint4x4,m' +
    'odf,msad4,mul,namespace,noExpressionOptimizations,nointerpolation,nois' +
    'e,NORMAL,NORMAL0,NORMAL1,NORMAL2,NORMAL3,NORMAL4,NORMAL5,NORMAL6,NORMA' +
    'L7,NORMAL8,NORMAL9,normalize,numthreads,out,OutputPatch,packoffset,poi' +
    'nt,PointStream,POSITION,POSITION0,POSITION1,POSITION2,POSITION3,POSITI' +
    'ON4,POSITION5,POSITION6,POSITION7,POSITION8,POSITION9,POSITIONT,pow,pr' +
    'ecise,predicate,predicateBlock,printf,Process2DQuadTessFactorsAvg,Proc' +
    'ess2DQuadTessFactorsMax,Process2DQuadTessFactorsMin,ProcessIsolineTess' +
    'Factors,ProcessQuadTessFactorsAvg,ProcessQuadTessFactorsMax,ProcessQua' +
    'dTessFactorsMin,ProcessTriTessFactorsAvg,ProcessTriTessFactorsMax,Proc' +
    'essTriTessFactorsMin,PSIZE,PSIZE0,PSIZE1,PSIZE2,PSIZE3,PSIZE4,PSIZE5,P' +
    'SIZE6,PSIZE7,PSIZE8,PSIZE9,radians,rcp,reduceTempRegUsage,reflect,refr' +
    'act,register,removeUnusedInputs,return,reversebits,round,row_major,rsq' +
    'rt,RWBuffer,RWByteAddressBuffer,RWStructuredBuffer,RWTexture1D,RWTextu' +
    're1DArray,RWTexture2D,RWTexture2DArray,RWTexture3D,sample,sampler,Samp' +
    'lerComparisonState,SamplerState,sampreg,saturate,shared,sign,sin,sinco' +
    's,sinh,smoothstep,snorm,sqrt,static,step,struct,StructuredBuffer,SV_Cl' +
    'ipDistance,SV_ClipDistance0,SV_ClipDistance1,SV_ClipDistance2,SV_ClipD' +
    'istance3,SV_ClipDistance4,SV_ClipDistance5,SV_ClipDistance6,SV_ClipDis' +
    'tance7,SV_ClipDistance8,SV_ClipDistance9,SV_Coverage,SV_CullDistance,S' +
    'V_CullDistance0,SV_CullDistance1,SV_CullDistance2,SV_CullDistance3,SV_' +
    'CullDistance4,SV_CullDistance5,SV_CullDistance6,SV_CullDistance7,SV_Cu' +
    'llDistance8,SV_CullDistance9,SV_Depth,SV_DispatchThreadID,SV_DomainLoc' +
    'ation,SV_GroupID,SV_GroupIndex,SV_GroupThreadID,SV_GSInstanceID,SV_Ins' +
    'ideTessFactor,SV_InstanceID,SV_IsFrontFace,SV_OutputControlPointID,SV_' +
    'Position,SV_PrimitiveID,SV_RenderTargetArrayIndex,SV_SampleIndex,SV_Ta' +
    'rget,SV_Target0,SV_Target1,SV_Target2,SV_Target3,SV_Target4,SV_Target5' +
    ',SV_Target6,SV_Target7,SV_TessFactor,SV_VertexID,SV_ViewportArrayIndex' +
    ',switch,tan,TANGENT,TANGENT0,TANGENT1,TANGENT2,TANGENT3,TANGENT4,TANGE' +
    'NT5,TANGENT6,TANGENT7,TANGENT8,TANGENT9,tanh,tbuffer,TESSFACTOR,TESSFA' +
    'CTOR0,TESSFACTOR1,TESSFACTOR2,TESSFACTOR3,TESSFACTOR4,TESSFACTOR5,TESS' +
    'FACTOR6,TESSFACTOR7,TESSFACTOR8,TESSFACTOR9,tex1D,tex1Dbias,tex1Dgrad,' +
    'tex1Dlod,tex1Dproj,tex2D,tex2Dbias,tex2Dgrad,tex2Dlod,tex2Dproj,tex3D,' +
    'tex3Dbias,tex3Dgrad,tex3Dlod,tex3Dproj,TEXCOORD,TEXCOORD0,TEXCOORD1,TE' +
    'XCOORD2,TEXCOORD3,TEXCOORD4,TEXCOORD5,TEXCOORD6,TEXCOORD7,TEXCOORD8,TE' +
    'XCOORD9,texCUBE,texCUBEbias,texCUBEgrad,texCUBElod,texCUBEproj,Texture' +
    '1D,Texture1DArray,Texture2D,Texture2DArray,Texture2DMS,Texture2DMSArra' +
    'y,Texture3D,TextureCube,TextureCubeArray,transpose,TriangleStream,true' +
    ',trunc,uint,uint1,uint1x1,uint1x2,uint1x3,uint1x4,uint2,uint2x1,uint2x' +
    '2,uint2x3,uint2x4,uint3,uint3x1,uint3x2,uint3x3,uint3x4,uint4,uint4x1,' +
    'uint4x2,uint4x3,uint4x4,uniform,unorm,unroll,unsigned,unused,vector,vo' +
    'id,volatile,while';
end;

function TSynHLSLSyn.GetTokenID: TtkTokenKind;
begin
  Result := fTokenId;
end;

function TSynHLSLSyn.GetTokenAttribute: TSynHighLighterAttributes;
begin
  case GetTokenID of
    tkBuiltInTypes: Result := fBuiltInTypesAttri;
    tkComment: Result := fCommentAttri;
    tkControlFlow: Result := fControlFlowAttri;
    tkFuncs: Result := fFuncsAttri;
    tkIdentifier: Result := fIdentifierAttri;
    tkKey: Result := fKeyAttri;
    tkMiscKey: Result := fMiscKeyAttri;
    tkObjs: Result := fObjsAttri;
    tkSemantic: Result := fSemanticAttri;
    tkShaderAttr: Result := fShaderAttrAttri;
    tkSpace: Result := fSpaceAttri;
    tkStorageClass: Result := fStorageClassAttri;
    tkString: Result := fStringAttri;
    tkTypeMod: Result := fTypeModAttri;
    tkVarUsage: Result := fVarUsageAttri;
    tkUnknown: Result := fIdentifierAttri;
  else
    Result := nil;
  end;
end;

function TSynHLSLSyn.GetTokenKind: Integer;
begin
  Result := Ord(fTokenId);
end;

function TSynHLSLSyn.IsIdentChar(AChar: WideChar): Boolean;
begin
  case AChar of
    '_', '0'..'9', 'a'..'z', 'A'..'Z':
      Result := True;
    else
      Result := False;
  end;
end;

function TSynHLSLSyn.GetSampleSource: UnicodeString;
begin
  Result := 
    '{ Sample source for the demo highlighter }'#13#10 +
    #13#10 +
    'This highlighter will recognize the words Hello and'#13#10 +
    'World as keywords. It will also highlight "Strings".'#13#10 +
    #13#10 +
    'And a special keyword type: SynEdit'#13#10 +
    '/* This style of comments is also highlighted */';
end;

function TSynHLSLSyn.IsFilterStored: Boolean;
begin
  Result := fDefaultFilter <> SYNS_FilterHLSL;
end;

class function TSynHLSLSyn.GetFriendlyLanguageName: UnicodeString;
begin
  Result := SYNS_FriendlyLangHLSL;
end;

class function TSynHLSLSyn.GetLanguageName: string;
begin
  Result := SYNS_LangHLSL;
end;

procedure TSynHLSLSyn.ResetRange;
begin
  fRange := rsUnknown;
end;

procedure TSynHLSLSyn.SetRange(Value: Pointer);
begin
  fRange := TRangeState(Value);
end;

function TSynHLSLSyn.GetRange: Pointer;
begin
  Result := Pointer(fRange);
end;

initialization
{$IFNDEF SYN_CPPB_1}
  RegisterPlaceableHighlighter(TSynHLSLSyn);
{$ENDIF}
end.
