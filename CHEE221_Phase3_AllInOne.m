%% CHEE221 Phase 3 - ALL-IN-ONE MATLAB FILE
% This file contains the Phase 3 main script AND all required functions as local functions.
% Just open this file in MATLAB and press Run.
% Generated: 2025-12-18


%% CHEE221 Phase 3 - Main Driver (Sequential + Recycle Iteration)
% Production of Propylene Glycol - Overall Process Simulation
%
% What this file does:
% 1) Solves the full steady-state flowsheet with a fixed recycle ratio R.
% 2) Study 1: effect of reactor temperature TR on PG production and utilities.
% 3) Study 2 ("free study"): effect of flash temperature TF on PG production and utilities.
%
% How to run:
% - Put all .m files from this package in the same folder.
% - In MATLAB, set Current Folder to that folder.
% - Run: Phase3_main
%
% Outputs:
% - Base case table printed in Command Window
% - Two study tables printed + exported to CSV
% - Plots saved as PNG files

clear; clc; close all;

%% 0) Component order (used everywhere)
%   1 = H2O
%   2 = PO  (propylene oxide)
%   3 = MeOH (methanol)
%   4 = PG  (propylene glycol)
comp = {'H2O','PO','MeOH','PG'};

%% 1) Base case parameters (edit these as needed)
params = defaultParams();   % all constants/specs in one place

% Fixed recycle ratio (fraction of condenser liquid returned to reactor)
R_recycle = 0.80;  % EDIT THIS if your team chose a different recycle ratio

% Solver options for recycle iteration
opts.maxIter   = 200;
opts.tolRel    = 1e-8;      % relative tolerance on recycle molar flow vector
opts.relax     = 0.50;      % under-relaxation factor (0-1). Try 0.3-0.7

%% 2) Fresh feeds (given in Part II specs)
% Stream 1: water
F1_m3h   = 5.0;
N1_molm3 = 0.366;            % mol/m^3
% Stream 2: propylene oxide
F2_m3h   = 1.0;
N2_molm3 = 0.020;
% Stream 3: methanol
F3_m3h   = 1.0;
N3_molm3 = 0.032;

nFresh = zeros(4,1);
nFresh(1) = F1_m3h * N1_molm3;   % H2O mol/h
nFresh(2) = F2_m3h * N2_molm3;   % PO mol/h
nFresh(3) = F3_m3h * N3_molm3;   % MeOH mol/h
nFresh(4) = 0.0;                % PG mol/h

fresh = makeLiquidStream(nFresh, params.T1_C, params.P_R_bar, params);

%% 3) Solve base case
base = solveFlowsheetSequentialRecycle(fresh, params, R_recycle, opts);

disp(' ');
disp('======================');
disp('BASE CASE (Converged)');
disp('======================');
disp(base.summaryTable);

%% 4) Study 1 - Effect of reactor temperature TR (C)
TR_vec = (45:5:70)';   % edit range if you want
studyTR = zeros(length(TR_vec), 7);

for i = 1:length(TR_vec)
    params_i = params;
    params_i.TR_C = TR_vec(i);
    sol = solveFlowsheetSequentialRecycle(fresh, params_i, R_recycle, opts);
    
    % Metrics to record
    PG_prod = sol.stream12.n(4);               % mol/h
    Qj      = sol.utilities.Q_reactor_kJh;     % kJ/h (+ means heating to reactor)
    Qflash  = sol.utilities.Q_flash_kJh;       % kJ/h
    Qcond   = sol.utilities.Q_cond_kJh;        % kJ/h (negative = removed)
    QHE     = sol.utilities.Q_HE_kJh;          % kJ/h (+ means heat added)
    mSteam  = sol.utilities.mSteam_kgph;       % kg/h
    
    studyTR(i,:) = [TR_vec(i), PG_prod, Qj, Qflash, Qcond, QHE, mSteam];
    fprintf('TR = %.1f C: PG = %.6g mol/h | Q_HE = %.6g kJ/h | iters=%d\n', ...
            TR_vec(i), PG_prod, QHE, sol.iterations);
end

tblTR = array2table(studyTR, ...
    'VariableNames', {'TR_C','PG_molph','Q_reactor_kJh','Q_flash_kJh','Q_cond_kJh','Q_HE_kJh','mSteam_kgph'});

disp(' ');
disp('============================');
disp('STUDY 1: Reactor Temperature');
disp('============================');
disp(tblTR);

writetable(tblTR, 'Study_TR_table.csv');

% Plots (saved)
figure; plot(tblTR.TR_C, tblTR.PG_molph, 'o-','LineWidth',1.5);
xlabel('Reactor Temperature, TR (C)'); ylabel('PG Production (mol/h)');
title('Effect of Reactor Temperature on PG Production');
grid on; exportgraphics(gcf,'Study_TR_PG.png','Resolution',300);

figure; plot(tblTR.TR_C, tblTR.Q_reactor_kJh, 's-','LineWidth',1.5);
xlabel('Reactor Temperature, TR (C)'); ylabel('Reactor Jacket Heat, Q_{reactor} (kJ/h)');
title('Effect of Reactor Temperature on Reactor Utility Heat');
grid on; exportgraphics(gcf,'Study_TR_Qreactor.png','Resolution',300);

figure; plot(tblTR.TR_C, tblTR.Q_HE_kJh, 'd-','LineWidth',1.5);
xlabel('Reactor Temperature, TR (C)'); ylabel('Heat Exchanger Duty, Q_{HE} (kJ/h)');
title('Effect of Reactor Temperature on Heat Exchanger Utility Heat');
grid on; exportgraphics(gcf,'Study_TR_QHE.png','Resolution',300);

%% 5) Study 2 - Effect of flash temperature TF (C)
TF_vec = (35:5:70)';   % edit range if you want
studyTF = zeros(length(TF_vec), 7);

for i = 1:length(TF_vec)
    params_i = params;
    params_i.TF_C = TF_vec(i);
    sol = solveFlowsheetSequentialRecycle(fresh, params_i, R_recycle, opts);
    
    PG_prod = sol.stream12.n(4);               % mol/h
    Qj      = sol.utilities.Q_reactor_kJh;     % kJ/h
    Qflash  = sol.utilities.Q_flash_kJh;       % kJ/h
    Qcond   = sol.utilities.Q_cond_kJh;        % kJ/h
    QHE     = sol.utilities.Q_HE_kJh;          % kJ/h
    mSteam  = sol.utilities.mSteam_kgph;       % kg/h
    
    studyTF(i,:) = [TF_vec(i), PG_prod, Qj, Qflash, Qcond, QHE, mSteam];
    fprintf('TF = %.1f C: PG = %.6g mol/h | Q_cond = %.6g kJ/h | iters=%d\n', ...
            TF_vec(i), PG_prod, Qcond, sol.iterations);
end

tblTF = array2table(studyTF, ...
    'VariableNames', {'TF_C','PG_molph','Q_reactor_kJh','Q_flash_kJh','Q_cond_kJh','Q_HE_kJh','mSteam_kgph'});

disp(' ');
disp('===========================');
disp('STUDY 2: Flash Temperature');
disp('===========================');
disp(tblTF);

writetable(tblTF, 'Study_TF_table.csv');

% Plots (saved)
figure; plot(tblTF.TF_C, tblTF.PG_molph, 'o-','LineWidth',1.5);
xlabel('Flash Temperature, TF (C)'); ylabel('PG Production (mol/h)');
title('Effect of Flash Temperature on PG Production');
grid on; exportgraphics(gcf,'Study_TF_PG.png','Resolution',300);

figure; plot(tblTF.TF_C, tblTF.Q_cond_kJh, 's-','LineWidth',1.5);
xlabel('Flash Temperature, TF (C)'); ylabel('Condenser Duty, Q_{cond} (kJ/h)');
title('Effect of Flash Temperature on Condenser Utility Heat');
grid on; exportgraphics(gcf,'Study_TF_Qcond.png','Resolution',300);

figure; plot(tblTF.TF_C, tblTF.Q_flash_kJh, 'd-','LineWidth',1.5);
xlabel('Flash Temperature, TF (C)'); ylabel('Flash Heat, Q_{flash} (kJ/h)');
title('Effect of Flash Temperature on Flash Energy Requirement');
grid on; exportgraphics(gcf,'Study_TF_Qflash.png','Resolution',300);

disp(' ');
disp('Done. CSV tables + PNG plots were saved in the current folder.');



%% =========================
%  LOCAL FUNCTIONS (auto-merged)
% =========================


function params = defaultParams()
% DEFAULTPARAMS - holds all constants/specifications in one place.
% Units:
%   Temperatures in Celsius unless specified
%   Pressures in bar
%   Flow rates in mol/h (inside stream structs)
%   Heat duties in kJ/h

% Universal gas constant
params.R_JmolK = 8.314;

% Reference temperature for sensible enthalpy calculations
params.Tref_C = 25.0;

% --- Reactor specs (Part II) ---
params.VR_m3    = 1.0;        % reactor volume
params.P_R_bar  = 4.5;        % 0.45 MPa = 4.5 bar
params.TR_C     = 60.0;       % nominal reactor temperature

% Fresh feed temperatures (Part II says T1=T2=T3=45C)
params.T1_C = 45.0;

% Kinetics (Part II)
params.k0 = 1.696e13;
params.E  = 75.3e3;           % J/mol (note: 75.3 kJ/mol)
% Rate form: r = k0 * exp(-E/(R*T)) * C_PO  where C_PO in mol/m^3, r in (mol/m^3)/h

% Jacket / heat transfer (Part II)
params.UA_kJK_h = 50.0;       % kJ/K/h
params.T9_C     = 80.0;       % jacket inlet water temperature
params.VJ_m3    = 0.4;        % jacket holdup volume (not used directly for steady-state)

% --- Flash specs (Part II) ---
params.PF_bar   = 1.5;        % 0.150 MPa = 1.5 bar
params.TF_C     = 45.0;       % nominal flash temperature

% --- Condenser spec (Part II) ---
% Total condenser, assume outlet temperature equals inlet vapor temperature
% (Part II: T8 = T6)
% no additional params needed

% --- Heat Exchanger spec (Part II) ---
params.T12_C     = 80.0;      % product outlet temperature
params.Psteam_MPa = 0.10;     % steam pressure
params.Hsteam150_kJkg = 2776.6;
params.Hsteam100_kJkg = 2675.8;

% Component properties (from Part II specs) --------------------------------
% Molecular weights (g/mol)
params.MW_gmol = [18.01528; 58.079; 32.041; 76.095];  % H2O, PO, MeOH, PG

% Liquid heat capacities Cp_liq (kJ/mol/K)
% Water: constant 75.4e-3
% Methanol: Cp_liq(T) = 75.86e-3 + 16.83e-5*T(K)
% PO: constant 0.1229
% PG: constant 0.1899
params.CpW_liq_kJmolK  = 75.4e-3;
params.CpPO_liq_kJmolK = 0.1229;
params.CpPG_liq_kJmolK = 0.1899;
params.CpM_a = 75.86e-3;      % kJ/mol/K
params.CpM_b = 16.83e-5;      % kJ/mol/K^2 (multiplies T in K)

% Vapor heat capacities Cp_vap (kJ/mol/K) for condenser/flash energy (simplified)
params.CpW_vap_poly = [33.46e-3, 0.6880e-5, 0.7604e-8, -3.593e-12]; % a + bT + cT^2 + dT^3 (T in K)
params.CpM_vap_poly = [42.93e-3, 8.301e-5, -1.87e-8, -8.03e-12];
params.CpPO_vap_kJmolK = 0.07255;
params.CpPG_vap_kJmolK = 0.1355;

% Enthalpies of vaporization (kJ/mol) (simplified constants)
params.Hvap_W_kJmol  = 40.656;
params.Hvap_M_kJmol  = 35.27;
params.Hvap_PO_kJmol = 27.9;
params.Hvap_PG_kJmol = 65.0;

end



function sol = solveFlowsheetSequentialRecycle(fresh, params, R_recycle, opts)
% SOLVEFLOWSHEETSEQUENTIALRECYCLE
% Sequential modular simulation with recycle (tear stream = recycle from condenser).
%
% Inputs:
%   fresh      - liquid stream struct (fresh combined feed: H2O, PO, MeOH, PG)
%   params     - parameters struct from defaultParams()
%   R_recycle  - recycle ratio (0 to 1) applied to condenser liquid stream
%   opts       - struct with fields: maxIter, tolRel, relax
%
% Outputs:
%   sol - struct with fields:
%       stream4, stream6, stream8, stream11, stream12, purge7
%       utilities: Q_reactor_kJh, Q_flash_kJh, Q_cond_kJh, Q_HE_kJh, mSteam_kgph, TJ_C, mJ_kgph
%       iterations, summaryTable

% Initialize recycle guess (start from zero recycle)
recycle = makeLiquidStream(zeros(4,1), params.TF_C, params.P_R_bar, params);

for k = 1:opts.maxIter
    
    % 1) Mix fresh + recycle to reactor feed
    feedR = mixLiquidStreams({fresh, recycle}, params);
    feedR.P_bar = params.P_R_bar;  % reactor pressure
    feedR.T_C   = params.T1_C;     % all feeds specified 45C in Part II
    
    % 2) Reactor (produces stream 4 at TR)
    [stream4, utilR] = reactorUnit(feedR, params);
    
    % 3) Flash (stream 4 -> vapor stream6 + liquid stream11)
    [stream6, stream11, Q_flash] = flashUnit(stream4, params);
    
    % 4) Condenser (stream6 vapor -> stream8 liquid)
    [stream8, Q_cond] = condenserUnit(stream6, params);
    
    % 5) Split recycle and purge
    recycle_new = scaleStream(stream8, R_recycle, params);
    purge7      = scaleStream(stream8, 1-R_recycle, params);
    
    % 6) Convergence check on recycle molar flow vector
    if hasConverged(recycle_new.n, recycle.n, opts.tolRel)
        recycle = recycle_new;
        break;
    end
    
    % 7) Under-relaxation
    recycle = makeLiquidStream(opts.relax*recycle_new.n + (1-opts.relax)*recycle.n, ...
                              recycle_new.T_C, recycle_new.P_bar, params);
end

% Heat exchanger: stream11 (liquid) -> stream12 at 80C
[stream12, Q_HE, mSteam] = heatExchangerUnit(stream11, params);

% Prepare output
sol.stream4  = stream4;
sol.stream6  = stream6;
sol.stream8  = stream8;
sol.purge7   = purge7;
sol.stream11 = stream11;
sol.stream12 = stream12;

sol.utilities.Q_reactor_kJh = utilR.Q_reactor_kJh;
sol.utilities.TJ_C          = utilR.TJ_C;
sol.utilities.mJ_kgph       = utilR.mJ_kgph;
sol.utilities.Q_flash_kJh   = Q_flash;
sol.utilities.Q_cond_kJh    = Q_cond;
sol.utilities.Q_HE_kJh      = Q_HE;
sol.utilities.mSteam_kgph   = mSteam;

sol.iterations = k;
sol.summaryTable = makeSummaryTable(sol);

end



function s = makeLiquidStream(n_molph, T_C, P_bar, params)
% Make a liquid stream struct.
s.n      = n_molph(:);   % mol/h vector (4x1)
s.T_C    = T_C;
s.P_bar  = P_bar;
s.phase  = 'L';
s.vdot_m3h = liquidVolFlow_m3h(s.n, params);
end



function s = makeVaporStream(n_molph, T_C, P_bar, params)
% Make a vapor stream struct (ideal gas volumetric flow estimate).
s.n      = n_molph(:);
s.T_C    = T_C;
s.P_bar  = P_bar;
s.phase  = 'V';
s.vdot_m3h = vaporVolFlow_m3h(s.n, T_C, P_bar, params);
end



function s2 = scaleStream(s, factor, params)
% Scale stream molar flows by factor (keeps T, P).
s2 = s;
s2.n = factor * s.n;
if s.phase == 'L'
    s2.vdot_m3h = liquidVolFlow_m3h(s2.n, params);
else
    s2.vdot_m3h = vaporVolFlow_m3h(s2.n, s2.T_C, s2.P_bar, params);
end
end



function mix = mixLiquidStreams(streamCell, params)
% Mix liquid streams assuming perfect mixing.
% Temperature is computed by Cp-weighted average (sensible enthalpy basis).

nTot = zeros(4,1);
Htot = 0.0;

for i = 1:length(streamCell)
    si = streamCell{i};
    nTot = nTot + si.n;
    Htot = Htot + enthalpyLiquid_kJh(si.n, si.T_C, params);
end

% Solve for mixed temperature by bisection (enthalpy is monotonic in T)
Tlow = -50; Thigh = 200;
for it = 1:60
    Tmid = 0.5*(Tlow+Thigh);
    Hmid = enthalpyLiquid_kJh(nTot, Tmid, params);
    if Hmid < Htot
        Tlow = Tmid;
    else
        Thigh = Tmid;
    end
end

mix = makeLiquidStream(nTot, 0.5*(Tlow+Thigh), streamCell{1}.P_bar, params);
end



function ok = hasConverged(xNew, xOld, tolRel)
% Relative convergence on vector.
den = max(1e-12, norm(xOld,2));
ok = (norm(xNew-xOld,2)/den) < tolRel;
end



function vdot = liquidVolFlow_m3h(n_molph, params)
% Convert molar flow to an approximate liquid volumetric flow using rho = 1000 kg/m^3.
MW = params.MW_gmol;              % g/mol
m_kgph = sum(n_molph .* MW)/1000; % kg/h
rho = 1000;                       % kg/m^3 (project assumption)
vdot = m_kgph / rho;              % m^3/h
end



function vdot = vaporVolFlow_m3h(n_molph, T_C, P_bar, params)
% Ideal gas volumetric flow estimate (m^3/h).
nTot = sum(n_molph);      % mol/h
T_K = T_C + 273.15;
P_Pa = P_bar * 1e5;
R = params.R_JmolK;
vdot = nTot * R * T_K / P_Pa;  % m^3/h
end



function [out4, util] = reactorUnit(feed, params)
% REACTORUNIT - steady-state CSTR with fixed TR.
% Reaction: PO + H2O -> PG (MeOH inert)
% Rate (Part II): r = k0 * exp(-75.3/(8.314*T(K))) * C_PO
% where C_PO is PO concentration (mol/m^3) in reactor (approx = outlet concentration)
%
% Outputs:
%   out4: liquid stream at TR and PR
%   util: struct with Q_reactor (kJ/h), TJ (C), mJ (kg/h)

TR_C = params.TR_C;
TR_K = TR_C + 273.15;

% Inlet molar flows (mol/h)
n_in = feed.n;
nW_in  = n_in(1);
nPO_in = n_in(2);
nM_in  = n_in(3);
nPG_in = n_in(4);

% Volumetric flow through reactor (m^3/h) - use inlet liquid volume estimate
vdot = max(1e-12, feed.vdot_m3h);   % m^3/h

% Kinetic constant k (1/h)
k = params.k0 * exp(-params.E/(params.R_JmolK*TR_K));  % (1/h) times??? depends on units
% The given expression produces k in 1/h if used with C in mol/m^3 (project assumption).
% Extent (mol/h) solved from:
%   extent = r*VR = k * (C_PO_out) * VR
%   C_PO_out = nPO_out / vdot,  nPO_out = nPO_in - extent
% => extent = k*(nPO_in - extent)/vdot * VR
alpha = k * params.VR_m3 / vdot;
extent = (alpha/(1+alpha)) * nPO_in;

% Enforce not consuming more than available reactants
extent = min(extent, nPO_in);
extent = min(extent, nW_in);

% Outlet molar flows
n_out = zeros(4,1);
n_out(1) = nW_in  - extent;
n_out(2) = nPO_in - extent;
n_out(3) = nM_in;
n_out(4) = nPG_in + extent;

out4 = makeLiquidStream(n_out, TR_C, params.P_R_bar, params);

% Reactor energy balance (sensible only, consistent with Part II data usage)
Hin  = enthalpyLiquid_kJh(n_in,  feed.T_C, params);
Hout = enthalpyLiquid_kJh(n_out, TR_C,    params);

Q_reactor = Hout - Hin;   % kJ/h positive means heat must be added to reactor

% Heat transfer relation between jacket (well-mixed at TJ) and reactor at TR:
% Q = UA*(TJ - TR)
TJ_C = TR_C + Q_reactor / params.UA_kJK_h;

% Jacket energy: mJ * Cp_mass * (T9 - TJ) = Q
% Use Cp_mass of water ~ 4.18 kJ/kg/K
Cp_mass = 4.18;  % kJ/kg/K
mJ = Q_reactor / (Cp_mass * (params.T9_C - TJ_C));  % kg/h

util.Q_reactor_kJh = Q_reactor;
util.TJ_C = TJ_C;
util.mJ_kgph = mJ;

end



function [vap6, liq11, Q_flash] = flashUnit(feed4, params)
% FLASHUNIT - Equilibrium flash at TF and PF using Raoult's law K-values.
% We assume ideal solution: K_i = Psat_i(TF) / PF
% Use Rachford-Rice to compute vapor fraction.
%
% Outputs:
%   vap6  - vapor stream to condenser (stream 6)
%   liq11 - liquid stream to heat exchanger (stream 11)
%   Q_flash - kJ/h, energy required to bring feed at T4 to equilibrium products at TF

TF_C = params.TF_C;
PF_bar = params.PF_bar;

nF = feed4.n;
nTot = sum(nF);

% If no flow, return zero streams
if nTot < 1e-15
    vap6  = makeVaporStream(zeros(4,1), TF_C, PF_bar, params);
    liq11 = makeLiquidStream(zeros(4,1), TF_C, PF_bar, params);
    Q_flash = 0.0;
    return;
end

z = nF / nTot;  % feed mole fractions

% K-values from Raoult's law
Psat = zeros(4,1);
Psat(1) = PsatWater_bar(TF_C);
Psat(2) = PsatPO_bar(TF_C);
Psat(3) = PsatMeOH_bar(TF_C);
Psat(4) = PsatPG_bar(TF_C);

K = Psat / PF_bar;

% Rachford-Rice function
RR = @(beta) sum( z .* (K-1) ./ (1 + beta.*(K-1)) );

% Determine vapor fraction beta in [0,1]
RR0 = RR(0);
RR1 = RR(1);

if RR0 < 0
    beta = 0.0; % all liquid
elseif RR1 > 0
    beta = 1.0; % all vapor
else
    beta = fzero(RR, [0,1]);
end

% Phase compositions
x = z ./ (1 + beta*(K-1));
y = K .* x;

% Normalize (for numerical safety)
x = x/sum(x);
y = y/sum(y);

nV = beta * nTot;
nL = (1-beta) * nTot;

nV_i = y * nV;
nL_i = x * nL;

vap6  = makeVaporStream(nV_i, TF_C, PF_bar, params);
liq11 = makeLiquidStream(nL_i, TF_C, PF_bar, params);

% Energy balance (to hold flash at TF)
% Feed enters at T4 (TR), assume liquid feed at feed4.T_C.
Hin = enthalpyLiquid_kJh(nF, feed4.T_C, params);
Hout = enthalpyVapor_kJh(nV_i, TF_C, params) + enthalpyLiquid_kJh(nL_i, TF_C, params);

Q_flash = Hout - Hin; % kJ/h (positive means add heat, negative remove heat)

end

% ---------- Saturation pressures in bar ----------

function Pbar = PsatWater_bar(TC)
% Antoine (water): log10 P*(mmHg) = 7.96681 - 1668.210/(T(C)+228)
PmmHg = 10^(7.96681 - 1668.210/(TC + 228.0));
Pbar = PmmHg * (1.01325/760.0);
end

function Pbar = PsatMeOH_bar(TC)
% Antoine (MeOH): log10 P*(mmHg) = 8.08097 - 1582.271/(T(C)+239.725)
PmmHg = 10^(8.08097 - 1582.271/(TC + 239.725));
Pbar = PmmHg * (1.01325/760.0);
end

function Pbar = PsatPO_bar(TC)
% Antoine (PO): log10 P*(bar) = 3.55046 - 802.487/(T(K) - 81.348)
TK = TC + 273.15;
Pbar = 10^(3.55046 - 802.487/(TK - 81.348));
end

function Pbar = PsatPG_bar(TC)
% Antoine (PG): log10 P*(bar) = 6.307936 - 2692.187/(T(K) - 17.94)
TK = TC + 273.15;
Pbar = 10^(6.307936 - 2692.187/(TK - 17.94));
end



function [liq8, Q_cond] = condenserUnit(vap6, params)
% CONDENSERUNIT - Total condenser: vapor -> liquid with same composition.
% Part II: T8 = T6, constant composition.
%
% Q_cond is negative (heat removed) if condensation occurs.

Tcond_C = vap6.T_C;
Pcond_bar = vap6.P_bar;

nV = vap6.n;

% Liquid outlet
liq8 = makeLiquidStream(nV, Tcond_C, Pcond_bar, params);

% Condenser duty: remove latent + sensible (at same T, just latent)
Q_cond = - latentHeat_kJh(nV, params);  % negative means removed from process

end

function Qlat = latentHeat_kJh(n_molph, params)
% Sum of component vaporization enthalpies at reference (simplified constant)
Hvap = [params.Hvap_W_kJmol; params.Hvap_PO_kJmol; params.Hvap_M_kJmol; params.Hvap_PG_kJmol];
Qlat = sum(n_molph .* Hvap);  % kJ/h
end



function [out12, Q_HE, mSteam_kgph] = heatExchangerUnit(liq11, params)
% HEATEXCHANGERUNIT - heats liquid product from flash (stream 11) to 80C (stream 12).
% Steam side: superheated steam at 0.1 MPa, 150C -> 100C (given enthalpies).
%
% Outputs:
%   out12        - liquid stream at T12
%   Q_HE         - kJ/h (positive means heat added to product)
%   mSteam_kgph  - kg/h required

T12_C = params.T12_C;

n = liq11.n;
Hin = enthalpyLiquid_kJh(n, liq11.T_C, params);
Hout = enthalpyLiquid_kJh(n, T12_C, params);

Q_HE = Hout - Hin;  % kJ/h

dHsteam = params.Hsteam150_kJkg - params.Hsteam100_kJkg;  % kJ/kg
mSteam_kgph = Q_HE / dHsteam;

out12 = makeLiquidStream(n, T12_C, liq11.P_bar, params);

end



function H = enthalpyLiquid_kJh(n_molph, T_C, params)
% Sensible enthalpy of a liquid stream relative to Tref.
% H = sum_i n_i * integral(Cp_i,liq dT)

Tref_K = params.Tref_C + 273.15;
T_K    = T_C + 273.15;

% Cp models (kJ/mol/K)
dT = T_K - Tref_K;

H = 0.0;

% Water, PO, PG constant Cp
H = H + n_molph(1) * params.CpW_liq_kJmolK  * dT;
H = H + n_molph(2) * params.CpPO_liq_kJmolK * dT;
H = H + n_molph(4) * params.CpPG_liq_kJmolK * dT;

% Methanol Cp_liq(T) = a + b*T(K)
a = params.CpM_a; b = params.CpM_b;
H = H + n_molph(3) * ( a*dT + 0.5*b*(T_K^2 - Tref_K^2) );

end



function H = enthalpyVapor_kJh(n_molph, T_C, params)
% Vapor enthalpy relative to Tref: sensible (Cp_vap) + latent (Hvap)

Tref_K = params.Tref_C + 273.15;
T_K    = T_C + 273.15;
dT = T_K - Tref_K;

H = 0.0;

% Water vapor Cp polynomial: a + bT + cT^2 + dT^3  (T in K)
aw = params.CpW_vap_poly(1); bw = params.CpW_vap_poly(2);
cw = params.CpW_vap_poly(3); dw = params.CpW_vap_poly(4);
Hw_sens = n_molph(1) * ( aw*dT + 0.5*bw*(T_K^2 - Tref_K^2) + (1/3)*cw*(T_K^3 - Tref_K^3) + 0.25*dw*(T_K^4 - Tref_K^4) );
H = H + Hw_sens + n_molph(1)*params.Hvap_W_kJmol;

% PO vapor
H = H + n_molph(2)*params.CpPO_vap_kJmolK*dT + n_molph(2)*params.Hvap_PO_kJmol;

% Methanol vapor Cp polynomial
am = params.CpM_vap_poly(1); bm = params.CpM_vap_poly(2);
cm = params.CpM_vap_poly(3); dm = params.CpM_vap_poly(4);
Hm_sens = n_molph(3) * ( am*dT + 0.5*bm*(T_K^2 - Tref_K^2) + (1/3)*cm*(T_K^3 - Tref_K^3) + 0.25*dm*(T_K^4 - Tref_K^4) );
H = H + Hm_sens + n_molph(3)*params.Hvap_M_kJmol;

% PG vapor (usually tiny)
H = H + n_molph(4)*params.CpPG_vap_kJmolK*dT + n_molph(4)*params.Hvap_PG_kJmol;

end



function T = makeSummaryTable(sol)
% Make a compact summary table for key stream molar flows.

comp = {'H2O','PO','MeOH','PG'}';

Snames = {'Stream4_ReactorOut','Stream6_FlashVapor','Stream11_FlashLiq','Stream8_CondenserOut','Stream12_Product','Purge7'};
streams = {sol.stream4, sol.stream6, sol.stream11, sol.stream8, sol.stream12, sol.purge7};

data = [];
for i = 1:numel(streams)
    data = [data, streams{i}.n(:)];
end

T = table(comp, data(:,1), data(:,2), data(:,3), data(:,4), data(:,5), data(:,6), ...
    'VariableNames', [{'Component'}, Snames]);
end
