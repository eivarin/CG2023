average = int(input("Inserir a distância média: "))
dif = int(input("Inserir a diferença das distâncias: "))
time = int(input("Inserir o tempo a demorar: "))

elipse_maj_ax = average + dif
elipse_min_ax = average - dif

mid_x_squared = ((elipse_maj_ax**2) * ((average**2) - (elipse_min_ax**2))) / (elipse_maj_ax**2 - elipse_min_ax**2)
mid_x = mid_x_squared ** 0.5
mid_z = (average**2 - mid_x_squared)**0.5

print(f"""<translate time="{time}" align="true" segments="10000">
    <point x="{elipse_maj_ax}" y="0" z="0" />
    <point x="{mid_x}" y="0" z="{mid_z}" />
    <point x="0" y="0" z="{elipse_min_ax}" />
    <point x="{-mid_x}" y="0" z="{mid_z}" />
    <point x="{-elipse_maj_ax}" y="0" z="0" />
    <point x="{-mid_x}" y="0" z="{-mid_z}" />
    <point x="0" y="0" z="{-elipse_min_ax}" />
    <point x="{mid_x}" y="0" z="{-mid_z}" />
</translate>""")
