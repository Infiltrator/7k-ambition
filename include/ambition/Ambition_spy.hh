/*
 * Seven Kingdoms: Ambition
 *
 * Copyright 2025 Tim Sviridov
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @file
 *
 * Header file for Ambition::Spy.
 */

#pragma once

class Button3D;
class Firm;
class Spy;
class Unit;
class Worker;


namespace Ambition::Spy {

extern Button3D assassinationButton;
extern Button3D cancelButton;
extern Button3D stealReportsButton;


double assassinationChanceEstimate(
  const ::Spy* _7kaaSpy,
  const Firm* _7kaaFirm,
  ::Unit* target
);

int bribeAmountEstimate(
  const ::Spy* _7kaaSpy,
  Worker& target
);
int bribeAmountEstimate(
  const ::Spy* _7kaaSpy,
  ::Unit* target
);

double stealReportEspaceChanceEstimate(
  const ::Spy* _7kaaSpy,
  const char report
);

} // namespace Ambition::Spy
