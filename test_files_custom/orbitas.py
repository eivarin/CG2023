def calculate_new_average(min_axis: int, average: int) -> int:
    new_average = ((average**2) / 2)**0.5
    while new_average > min_axis:
        new_average = ((new_average**2) / 2)**0.5
    return new_average

average = int(input("Inserir a distância média: "))
dif = int(input("Inserir a diferença das distâncias: "))
time = int(input("Inserir o tempo a demorar: "))
highest = int(input("Inserir altura máxima: "))
lowest = int(input("Inserir altura mínima: "))

elipse_maj_ax = average + dif
elipse_min_ax = average - dif
new_average = calculate_new_average(elipse_min_ax, average)

height_mid = (highest + lowest) / 2
height_high_mid = (highest + height_mid) / 2
height_low_mid = (lowest + height_mid) / 2

print(f"""<translate time="{time}" align="true" segments="10000">
    <point x="{elipse_maj_ax}" y="{highest}" z="0" />
    <point x="{new_average}" y="{height_high_mid}" z="{new_average}" />
    <point x="0" y="{height_mid}" z="{elipse_min_ax}" />
    <point x="{-new_average}" y="{height_low_mid}" z="{new_average}" />
    <point x="{-elipse_maj_ax}" y="{lowest}" z="0" />
    <point x="{-new_average}" y="{height_low_mid}" z="{-new_average}" />
    <point x="0" y="{height_mid}" z="{-elipse_min_ax}" />
    <point x="{new_average}" y="{height_high_mid}" z="{-new_average}" />
</translate>""")
