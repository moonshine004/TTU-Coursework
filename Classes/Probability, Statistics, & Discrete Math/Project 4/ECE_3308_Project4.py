# %%
from datetime import datetime
now = datetime.now()
print(f"Last updated: {now.month}/{now.day}/{now.year} {now.strftime('%I:%M%p').lstrip('0').lower()}")
# %% [markdown]
# ## Block 0 - Input
# 
# This block imports the required libraries, defines file paths, and sets the main project parameters such as the target season, number of Monte Carlo simulations, and random seed.
# %%
# Block 0 - Input

import pandas as pd
import numpy as np
import re

from collections import defaultdict, Counter
from sklearn.linear_model import LogisticRegression
from sklearn.preprocessing import StandardScaler
from sklearn.metrics import accuracy_score, log_loss, confusion_matrix

import matplotlib.pyplot as plt
from matplotlib.backends.backend_pdf import PdfPages

# File paths
MAT_FILE = "Master_Analytical_Table.csv"
SEEDS_FILE = "MNCAATourneySeeds.csv"
SLOTS_FILE = "MNCAATourneySlots.csv"
SPELLINGS_FILE = "MTeamSpellings.csv"

TARGET_SEASON = 2025
N_SIMS = 10000
RANDOM_STATE = 42

np.random.seed(RANDOM_STATE)
# %% [markdown]
# ## Block 1 - Load Data
# 
# This block loads the Master Analytical Table, tournament seeds, tournament slots, and team spelling files that will be used for model training and tournament simulation.
# %%
# Block 1 - Load Data

mat = pd.read_csv(MAT_FILE)
seeds = pd.read_csv(SEEDS_FILE)
slots = pd.read_csv(SLOTS_FILE)
spellings = pd.read_csv(SPELLINGS_FILE)

print("MAT shape:", mat.shape)
print("Seeds shape:", seeds.shape)
print("Slots shape:", slots.shape)
print("Spellings shape:", spellings.shape)

display(mat.head())
display(seeds.head())
display(slots.head())
display(spellings.head())
# %% [markdown]
# ## Block 2 - Inspect and Prepare MAT Data
# 
# This block standardizes the MAT column names, confirms the required feature and target columns exist, and prepares the modeling dataframe used to train the logistic regression model.
# %%
# Block 2 - Inspect and Prepare MAT Data

print("Original MAT columns:")
print(mat.columns.tolist())

# Standardize MAT column names
mat.columns = mat.columns.str.strip().str.lower()

print("\nLowercase MAT columns:")
print(mat.columns.tolist())

# Required columns
feature_cols = ["diff_efg", "diff_tov", "diff_orb", "diff_ftr"]
required_cols = ["season", "teama_id", "teamb_id", "win_a"] + feature_cols

missing_cols = [col for col in required_cols if col not in mat.columns]
if missing_cols:
    raise ValueError(f"Missing required MAT columns: {missing_cols}")

model_df = mat[required_cols].copy()
model_df = model_df.dropna().sort_values("season").reset_index(drop=True)

print("\nPrepared modeling dataframe shape:", model_df.shape)
print("Feature columns:", feature_cols)

display(model_df.head())
print("\nSeason counts:")
print(model_df["season"].value_counts().sort_index().tail(10))
# %% [markdown]
# ## Block 3 - Train Logistic Regression Model
# 
# This block trains a logistic regression model using a chronological split by season. The features are scaled using StandardScaler, and the model is evaluated using accuracy, log-loss, confusion matrix, and AUC.
# %%
# Block 3 - Train Logistic Regression Model

from sklearn.metrics import accuracy_score, log_loss, confusion_matrix, roc_auc_score, roc_curve

available_seasons = sorted(model_df["season"].unique())
print("Available seasons:", available_seasons)

train_mask = model_df["season"] < TARGET_SEASON

test_candidates = [s for s in available_seasons if s < TARGET_SEASON]
if len(test_candidates) == 0:
    raise ValueError("No historical seasons available before TARGET_SEASON.")

test_season = max(test_candidates)
test_mask = model_df["season"] == test_season

X_train = model_df.loc[train_mask, feature_cols].copy()
y_train = model_df.loc[train_mask, "win_a"].copy()

X_test = model_df.loc[test_mask, feature_cols].copy()
y_test = model_df.loc[test_mask, "win_a"].copy()

print("Training seasons:", sorted(model_df.loc[train_mask, "season"].unique()))
print("Test season:", test_season)
print("X_train shape:", X_train.shape)
print("X_test shape:", X_test.shape)

scaler = StandardScaler()
X_train_scaled = scaler.fit_transform(X_train)
X_test_scaled = scaler.transform(X_test)

log_reg = LogisticRegression(max_iter=1000, random_state=RANDOM_STATE)
log_reg.fit(X_train_scaled, y_train)

test_probs = log_reg.predict_proba(X_test_scaled)[:, 1]
test_preds = (test_probs >= 0.5).astype(int)

acc = accuracy_score(y_test, test_preds)
ll = log_loss(y_test, test_probs)
cm = confusion_matrix(y_test, test_preds)
auc = roc_auc_score(y_test, test_probs)

print("\nModel Evaluation")
print("Accuracy:", round(acc, 4))
print("Log Loss:", round(ll, 4))
print("Confusion Matrix:\n", cm)
print("AUC:", round(auc, 4))

fpr, tpr, _ = roc_curve(y_test, test_probs)
plt.figure(figsize=(6, 5))
plt.plot(fpr, tpr, label=f"AUC = {auc:.4f}")
plt.plot([0, 1], [0, 1], linestyle="--")
plt.xlabel("False Positive Rate")
plt.ylabel("True Positive Rate")
plt.title("ROC Curve")
plt.legend()
plt.show()

coef_df = pd.DataFrame({
    "Feature": feature_cols,
    "Coefficient": log_reg.coef_[0]
}).sort_values("Coefficient", ascending=False).reset_index(drop=True)

print("\nModel Coefficients")
display(coef_df)
# %%
# Block 4 - Build 2025 Bracket Inputs

# Standardize other file column names
seeds.columns = seeds.columns.str.strip()
slots.columns = slots.columns.str.strip()
spellings.columns = spellings.columns.str.strip()

def seed_to_int(seed_str):
    match = re.search(r"(\d+)", str(seed_str))
    return int(match.group(1)) if match else np.nan

def slot_sort_key(slot_name):
    """
    Sort play-in slots first, then normal tournament rounds in order.

    Examples:
    W16, X11, Y11, Y16 -> play-in games, should come first
    R1W1 -> round 1
    R2W1 -> round 2
    ...
    R6CH -> championship
    """
    slot_name = str(slot_name)

    # Main bracket rounds
    if slot_name.startswith("R"):
        match = re.match(r"R(\d+)", slot_name)
        if match:
            return int(match.group(1))

    # Play-in slots like W16, X11, Y11, Y16
    if re.match(r"^[WXYZ]\d+", slot_name):
        return 0

    return 99

# Team names
team_name_map = (
    spellings.groupby("TeamID")["TeamNameSpelling"]
    .first()
    .reset_index()
    .rename(columns={"TeamNameSpelling": "TeamName"})
)

# 2025 seeds
seeds_2025 = seeds[seeds["Season"] == TARGET_SEASON].copy()
seeds_2025["SeedNum"] = seeds_2025["Seed"].apply(seed_to_int)
seeds_2025 = seeds_2025.merge(team_name_map, on="TeamID", how="left")

print("2025 tournament teams:", len(seeds_2025))
display(seeds_2025.head())

# 2025 slots
if "Season" in slots.columns:
    slots_2025 = slots[slots["Season"] == TARGET_SEASON].copy()
else:
    slots_2025 = slots.copy()

slots_2025["SortOrder"] = slots_2025["Slot"].apply(slot_sort_key)
slots_2025 = slots_2025.sort_values(["SortOrder", "Slot"]).reset_index(drop=True)

print("2025 slot rows:", len(slots_2025))
display(slots_2025.head(20))
display(slots_2025.tail(10))

# Lookup dictionaries
seed_to_team_2025 = dict(zip(seeds_2025["Seed"], seeds_2025["TeamID"]))
team_to_seednum_2025 = dict(zip(seeds_2025["TeamID"], seeds_2025["SeedNum"]))
team_to_name_2025 = dict(zip(seeds_2025["TeamID"], seeds_2025["TeamName"]))
team_to_seedlabel_2025 = dict(zip(seeds_2025["TeamID"], seeds_2025["Seed"]))
# %% [markdown]
# ## Block 5 - Define Matchup Probability Functions
# 
# This block defines helper functions that build the feature differences for any possible matchup and use the trained logistic regression model to predict the probability that one team beats another.
# %%
# Block 5 - Define Matchup Probability Functions

# Build 2025 team profiles from MAT by averaging rows where each team appears
team_a_view = model_df[model_df["season"] == TARGET_SEASON][["teama_id"] + feature_cols].copy()
team_a_view = team_a_view.rename(columns={"teama_id": "team_id"})

team_b_view = model_df[model_df["season"] == TARGET_SEASON][["teamb_id"] + feature_cols].copy()
team_b_view = team_b_view.rename(columns={"teamb_id": "team_id"})

# Reverse signs for team B rows so all rows reflect that team's own profile direction
for col in feature_cols:
    team_b_view[col] = -team_b_view[col]

team_profiles = pd.concat([team_a_view, team_b_view], ignore_index=True)

team_profiles_2025 = (
    team_profiles.groupby("team_id", as_index=False)[feature_cols]
    .mean()
)

team_profiles_2025 = team_profiles_2025.merge(
    seeds_2025[["TeamID", "SeedNum", "Seed", "TeamName"]],
    left_on="team_id",
    right_on="TeamID",
    how="inner"
)

team_feat_lookup = {}
for _, row in team_profiles_2025.iterrows():
    team_feat_lookup[int(row["team_id"])] = {
        "diff_efg": row["diff_efg"],
        "diff_tov": row["diff_tov"],
        "diff_orb": row["diff_orb"],
        "diff_ftr": row["diff_ftr"],
        "SeedNum": row["SeedNum"],
        "Seed": row["Seed"],
        "TeamName": row["TeamName"]
    }

print("Team profiles available for 2025:", len(team_feat_lookup))
display(team_profiles_2025.head())

def resolve_source(source, winners_dict):
    source = str(source)

    if source in winners_dict:
        return winners_dict[source]
    if source in seed_to_team_2025:
        return seed_to_team_2025[source]

    return None

def build_matchup_row(team_a, team_b):
    a = team_feat_lookup[team_a]
    b = team_feat_lookup[team_b]

    return pd.DataFrame([{
        "diff_efg": a["diff_efg"] - b["diff_efg"],
        "diff_tov": a["diff_tov"] - b["diff_tov"],
        "diff_orb": a["diff_orb"] - b["diff_orb"],
        "diff_ftr": a["diff_ftr"] - b["diff_ftr"]
    }])

# Cache matchup probabilities so repeated matchups are only computed once
prob_cache = {}

def predict_win_prob(team_a, team_b):
    key = (team_a, team_b)

    if key in prob_cache:
        return prob_cache[key]

    X = build_matchup_row(team_a, team_b)[feature_cols]
    X_scaled = scaler.transform(X)
    prob = float(log_reg.predict_proba(X_scaled)[0, 1])

    prob_cache[key] = prob
    prob_cache[(team_b, team_a)] = 1.0 - prob

    return prob

def round_label_from_slot(slot_name):
    slot_name = str(slot_name)

    # Championship
    if "CH" in slot_name:
        return "Champ"

    # Final Four / national semifinal
    if slot_name.startswith("R5"):
        return "Final4"

    # Elite Eight
    if slot_name.startswith("R4"):
        return "Elite8"

    # Sweet 16
    if slot_name.startswith("R3"):
        return "Sweet16"

    # Round of 32
    if slot_name.startswith("R2"):
        return "R32"

    # First round winners also count as reaching Round of 32
    if slot_name.startswith("R1"):
        return "R32"

    return "Other"

# Optional warm-up: precompute initial round probabilities
for _, row in slots_2025.iterrows():
    strong_src = str(row["StrongSeed"])
    weak_src = str(row["WeakSeed"])

    if strong_src in seed_to_team_2025 and weak_src in seed_to_team_2025:
        team_a = seed_to_team_2025[strong_src]
        team_b = seed_to_team_2025[weak_src]
        _ = predict_win_prob(team_a, team_b)

print("Initial cached matchups:", len(prob_cache))
# %% [markdown]
# ## Block 6 - Simulate One Tournament
# 
# This block simulates one complete tournament by resolving each slot in bracket order, generating matchup probabilities, choosing winners with random sampling, and recording how far each team advances.
# %%
# Block 6 - Simulate One Tournament

def simulate_one_tournament(slots_frame):
    winners_dict = {}
    advancement = defaultdict(lambda: {
        "R32": 0,
        "Sweet16": 0,
        "Elite8": 0,
        "Final4": 0,
        "Champ": 0
    })
    matchup_log = []

    for _, row in slots_frame.iterrows():
        slot = row["Slot"]
        strong_src = row["StrongSeed"]
        weak_src = row["WeakSeed"]

        team_a = resolve_source(strong_src, winners_dict)
        team_b = resolve_source(weak_src, winners_dict)

        if team_a is None or team_b is None:
            continue

        p_a = predict_win_prob(team_a, team_b)
        winner = team_a if np.random.rand() < p_a else team_b
        loser = team_b if winner == team_a else team_a

        winners_dict[slot] = winner

        r_label = round_label_from_slot(slot)
        if r_label in advancement[winner]:
            advancement[winner][r_label] = 1

        matchup_log.append({
            "Slot": slot,
            "Round": r_label,
            "TeamA": team_a,
            "TeamB": team_b,
            "ProbA": p_a,
            "Winner": winner,
            "Loser": loser
        })

    return winners_dict, advancement, matchup_log

test_winners, test_advancement, test_log = simulate_one_tournament(slots_2025)

print("Single simulation complete.")
print("Resolved slots:", len(test_winners))

if len(test_log) > 0:
    display(pd.DataFrame(test_log).head())
# %% [markdown]
# ## Block 7 - Run Monte Carlo Simulation
# 
# This block repeats the tournament simulation 10,000 times and tracks how often each team reaches each round. It also stores matchup outcome counts that will be used later to construct the final optimal bracket.
# %%
# Block 7 - Run Monte Carlo Simulation

all_team_ids = sorted(team_profiles_2025["team_id"].unique())

results_counter = {
    team_id: {"R32": 0, "Sweet16": 0, "Elite8": 0, "Final4": 0, "Champ": 0}
    for team_id in all_team_ids
}

round1_win_counter = Counter()
pair_win_counter = Counter()

for sim in range(N_SIMS):
    winners_dict, advancement, matchup_log = simulate_one_tournament(slots_2025)

    for team_id, round_dict in advancement.items():
        for rnd, val in round_dict.items():
            results_counter[team_id][rnd] += val

    for item in matchup_log:
        team_a = item["TeamA"]
        team_b = item["TeamB"]
        winner = item["Winner"]
        slot = item["Slot"]
        rnd = item["Round"]

        if rnd == "R32":
            round1_key = (slot, winner)
            round1_win_counter[round1_key] += 1

        matchup_pair = tuple(sorted([team_a, team_b]))
        pair_key = (rnd, matchup_pair, winner)
        pair_win_counter[pair_key] += 1

    if (sim + 1) % 1000 == 0:
        print(f"Completed {sim + 1} / {N_SIMS} simulations")

print("Monte Carlo simulation finished.")
# %% [markdown]
# ## Block 8 - Build Frequency Table
# 
# This block converts the raw simulation counts into round advancement probabilities for each team and creates the required frequency table sorted by championship probability.
# %%
# Block 8 - Build Frequency Table

freq_rows = []

for team_id in all_team_ids:
    info = team_feat_lookup[team_id]
    freq_rows.append({
        "TeamName": info["TeamName"],
        "Seed": info["SeedNum"],
        "R32_Prob": results_counter[team_id]["R32"] / N_SIMS,
        "Sweet16_Prob": results_counter[team_id]["Sweet16"] / N_SIMS,
        "Elite8_Prob": results_counter[team_id]["Elite8"] / N_SIMS,
        "Final4_Prob": results_counter[team_id]["Final4"] / N_SIMS,
        "Champ_Prob": results_counter[team_id]["Champ"] / N_SIMS
    })

frequency_table = pd.DataFrame(freq_rows).sort_values("Champ_Prob", ascending=False).reset_index(drop=True)

display(frequency_table.head(20))
print("Champ_Prob sum:", round(frequency_table["Champ_Prob"].sum(), 6))
# %% [markdown]
# ## Block 9 - Cinderella Report
# 
# This block identifies a potential Cinderella team by searching for teams seeded 10 or worse that still have a strong probability of reaching the Sweet 16. It then generates a short explanation based on the team’s statistical profile.
# %% [markdown]
# ### Cinderella Pick
# 
# UC San Diego (Seed 12) emerges as the model’s Cinderella team.
# Despite their lower seed, the simulation predicts they reach the Sweet 16 in over 71% of runs and even make the championship in some scenarios.
# 
# This suggests their statistical profile, particularly shooting efficiency and rebounding strength, produces favorable matchups in the Monte Carlo simulations.
# %%
# Block 9 - Cinderella Report

cinderella_candidates = frequency_table[
    (frequency_table["Seed"] >= 10) &
    (frequency_table["Sweet16_Prob"] > 0.20)
].copy().sort_values(["Sweet16_Prob", "Champ_Prob"], ascending=False)

display(cinderella_candidates)

avg_efg = team_profiles_2025["diff_efg"].mean()
avg_tov = team_profiles_2025["diff_tov"].mean()
avg_orb = team_profiles_2025["diff_orb"].mean()
avg_ftr = team_profiles_2025["diff_ftr"].mean()

def build_cinderella_explanation(team_name):
    row = team_profiles_2025[team_profiles_2025["TeamName"] == team_name].iloc[0]
    reasons = []

    if row["diff_efg"] > avg_efg:
        reasons.append("above-average shooting efficiency")
    if row["diff_tov"] > avg_tov:
        reasons.append("a favorable turnover profile")
    if row["diff_orb"] > avg_orb:
        reasons.append("strong offensive rebounding potential")
    if row["diff_ftr"] > avg_ftr:
        reasons.append("an ability to generate free throws")

    if len(reasons) == 0:
        reasons.append("a balanced statistical profile compared to the field")

    return "This team stands out because it has " + ", ".join(reasons) + "."

if len(cinderella_candidates) > 0:
    best_cinderella = cinderella_candidates.iloc[0]
    explanation = build_cinderella_explanation(best_cinderella["TeamName"])

    cinderella_report = pd.DataFrame([{
        "TeamName": best_cinderella["TeamName"],
        "Seed": int(best_cinderella["Seed"]),
        "Sweet16_Prob": best_cinderella["Sweet16_Prob"],
        "Elite8_Prob": best_cinderella["Elite8_Prob"],
        "Final4_Prob": best_cinderella["Final4_Prob"],
        "Champ_Prob": best_cinderella["Champ_Prob"],
        "Why": explanation
    }])
else:
    cinderella_report = pd.DataFrame([{
        "TeamName": "No qualifying team found",
        "Seed": np.nan,
        "Sweet16_Prob": np.nan,
        "Elite8_Prob": np.nan,
        "Final4_Prob": np.nan,
        "Champ_Prob": np.nan,
        "Why": "No seed 10 or worse exceeded a 20% Sweet 16 probability in this simulation."
    }])

display(cinderella_report)
# %% [markdown]
# ## Block 10 - Build Optimal Bracket
# 
# This block constructs the final bracket prediction. Round 1 winners are chosen using the greater than 50 percent probability rule, and later round winners are chosen using the most frequent simulated winner for each possible matchup.
# %%
# Block 10 - Build Optimal Bracket

def choose_later_round_winner(team_a, team_b, round_name):
    matchup_pair = tuple(sorted([team_a, team_b]))
    a_count = pair_win_counter[(round_name, matchup_pair, team_a)]
    b_count = pair_win_counter[(round_name, matchup_pair, team_b)]

    if a_count > b_count:
        return team_a
    if b_count > a_count:
        return team_b

    p_a = predict_win_prob(team_a, team_b)
    return team_a if p_a >= 0.5 else team_b

optimal_winners = {}
optimal_rows = []

for _, row in slots_2025.iterrows():
    slot = row["Slot"]
    strong_src = row["StrongSeed"]
    weak_src = row["WeakSeed"]

    team_a = resolve_source(strong_src, optimal_winners)
    team_b = resolve_source(weak_src, optimal_winners)

    if team_a is None or team_b is None:
        continue

    rnd = round_label_from_slot(slot)
    p_a = predict_win_prob(team_a, team_b)

    if rnd == "R32":
        winner = team_a if p_a >= 0.5 else team_b
    else:
        winner = choose_later_round_winner(team_a, team_b, rnd)

    optimal_winners[slot] = winner

    optimal_rows.append({
        "Slot": slot,
        "Round": rnd,
        "TeamA": team_feat_lookup[team_a]["TeamName"],
        "SeedA": team_feat_lookup[team_a]["Seed"],
        "TeamB": team_feat_lookup[team_b]["TeamName"],
        "SeedB": team_feat_lookup[team_b]["Seed"],
        "Prob_TeamA_Wins": p_a,
        "Predicted_Winner": team_feat_lookup[winner]["TeamName"],
        "WinnerSeed": team_feat_lookup[winner]["Seed"]
    })

optimal_bracket_df = pd.DataFrame(optimal_rows)
display(optimal_bracket_df)
# %% [markdown]
# ## Block 11 - Export Outputs
# 
# This block exports the frequency table, Cinderella report, and optimal bracket to CSV files. It also creates a simple PDF summary of the final bracket results for submission.
# %%
# Block 11 - Export Outputs

frequency_table.to_csv("Project4_FrequencyTable.csv", index=False)
cinderella_report.to_csv("Project4_CinderellaReport.csv", index=False)
optimal_bracket_df.to_csv("Project4_OptimalBracket.csv", index=False)

print("CSV files exported.")

with PdfPages("Project4_FinalBracket.pdf") as pdf:
    fig, ax = plt.subplots(figsize=(11, 8.5))
    ax.axis("off")

    top16 = frequency_table.head(16).copy()
    top16["Champ_Prob"] = top16["Champ_Prob"].round(4)

    text_lines = ["Project 4 - Final Bracket Report", "", "Top 16 Championship Odds", ""]
    for _, row in top16.iterrows():
        text_lines.append(f"{row['TeamName']} (Seed {int(row['Seed'])}) - Champ Prob: {row['Champ_Prob']}")

    ax.text(0.02, 0.98, "\n".join(text_lines), va="top", fontsize=11)
    pdf.savefig(fig, bbox_inches="tight")
    plt.close(fig)

    fig, ax = plt.subplots(figsize=(11, 8.5))
    ax.axis("off")

    round_order = ["R32", "Sweet16", "Elite8", "Final4", "Champ"]
    lines = ["Predicted Winners by Round", ""]

    for rnd in round_order:
        lines.append(rnd)
        subset = optimal_bracket_df[optimal_bracket_df["Round"] == rnd]
        for _, row in subset.iterrows():
            lines.append(
                f"  {row['Slot']}: {row['Predicted_Winner']} "
                f"from {row['TeamA']} vs {row['TeamB']}"
            )
        lines.append("")

    ax.text(0.02, 0.98, "\n".join(lines), va="top", fontsize=9)
    pdf.savefig(fig, bbox_inches="tight")
    plt.close(fig)

print("PDF exported: Project4_FinalBracket.pdf")
# %% [markdown]
# ## Block 12 - Final Checks
# 
# This block performs a few final checks to confirm that the frequency table, Cinderella report, and optimal bracket were generated successfully and that the championship probabilities sum correctly.
# %%
# Block 12 - Final Checks

print("Frequency table rows:", len(frequency_table))
print("Optimal bracket rows:", len(optimal_bracket_df))
print("Cinderella report rows:", len(cinderella_report))

print("\nTop 10 title odds:")
display(frequency_table.head(10))

print("\nCinderella report:")
display(cinderella_report)

print("\nPredicted champion:")
champ_rows = optimal_bracket_df[optimal_bracket_df["Round"] == "Champ"]
display(champ_rows)

print("\nChamp_Prob sum should be about 1.0")
print(round(frequency_table["Champ_Prob"].sum(), 6))